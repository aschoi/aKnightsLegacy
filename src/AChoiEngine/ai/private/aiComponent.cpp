#include "AChoiEngine/ai/public/aiComponent.h"
#include <array>
#include <vector>
#include <queue>
#include <unordered_set>
#include <limits>
#include "AChoiEngine/gameObject/public/GameObject.h"
#include "AChoiEngine/worldLayer/public/LdtkMapLoader.h"
#include "AChoiEngine/worldLayer/public/TxtMapLoader.h"
#include "AChoiEngine/worldLayer/public/Map.h"


struct PairHash {
    size_t operator()(const std::pair<int, int>& p) const noexcept {
        return std::hash<int>{}(p.first) ^ (std::hash<int>{}(p.second) << 1);
    }
};
struct Array3Hash {
    size_t operator()(const std::array<int, 3>& a) const noexcept {
        size_t h = 0;
        // simple combine
        h ^= std::hash<int>{}(a[0]) + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
        h ^= std::hash<int>{}(a[1]) + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
        h ^= std::hash<int>{}(a[2]) + 0x9e3779b97f4a7c15ULL + (h << 6) + (h >> 2);
        return h;
    }
};


void followTargetSimple(GameObject& hunter, GameObject& target, Map& curMap, float speed) {
    float xDiff = target.get_x_px() - hunter.get_x_px();
    float yDiff = target.get_y_px() - hunter.get_y_px();

    float followSpeed = speed;
    float dt = 1.0 / 60.0; // events/second aka hz
    float distSq = (xDiff * xDiff) + (yDiff * yDiff);

    // In zone -> move toward player
    if (distSq < 0.000001f) return; // already at same point

    float dist = std::sqrt(distSq);
    float invDist = 1.0f / dist;

    float dirX = xDiff * invDist;
    float dirY = yDiff * invDist;

    float oldX = hunter.get_x_px();
    float oldY = hunter.get_y_px();

    hunter.set_x_px(hunter.get_x_px() + (dirX * followSpeed * dt));
    hunter.set_y_px(hunter.get_y_px() + (dirY * followSpeed * dt));

    int tl = curMap.intGridType(hunter.get_x_px(), hunter.get_y_px());
    int tr = curMap.intGridType(hunter.get_x_px() + 8, hunter.get_y_px());
    int bl = curMap.intGridType(hunter.get_x_px(), hunter.get_y_px() + 36);
    int br = curMap.intGridType(hunter.get_x_px() + 8, hunter.get_y_px() + 36);

    if (tl > 0 || tr > 0 || bl > 0 || br > 0) {
        hunter.set_x_px(oldX);
        hunter.set_y_px(oldY);
    }

    hunter.set_x_gu(static_cast<int>(std::floor(hunter.get_x_px() / hunter.get_singleGU_sideLen_inPixels())));
    hunter.set_y_gu(static_cast<int>(std::floor(hunter.get_y_px() / hunter.get_singleGU_sideLen_inPixels())));

}


std::vector<std::vector<int>> CreateCostMap(Map& curMap, GameObject& player) {

    // 8 directions
    constexpr std::array<std::array<int, 2>, 8> dirs{ {{0, 1}, {0, -1}, {1, 0}, {-1, 0},
                                                       {1, 1}, {1, -1}, {-1, 1}, {-1, -1}}
    };

    const int h_gUnits = curMap.getWorldHeight_gridUnits(); // rows
    const int w_gUnits = curMap.getWorldWidth_gridUnits();  // cols

    std::vector<std::vector<int>> costMap(h_gUnits,
        std::vector<int>(w_gUnits, -1)
    );

    // Potential Optimization: replace q with another Data Structure
    std::queue<std::array<int, 3>> q;
    q.push({ player.get_y_gu(), player.get_x_gu(), 0 });

    std::unordered_set<std::pair<int, int>, PairHash> visited;
    costMap[player.get_y_gu()][player.get_x_gu()] = 0;
    visited.insert({ player.get_y_gu(), player.get_x_gu() });

    while (!q.empty()) {

        auto [curR, curC, curDist] = q.front();
        q.pop();

        for (const auto& [dr, dc] : dirs) {
            int nr = curR + dr;
            int nc = curC + dc;

            // for the curMap cost, its avoiding 1's and 2's. If the ldtk intGridCsv grid is a 1 or 2, ai will avoid those 
            // specific numbers. Can modify this to adapt which numbers its avoids, but that how this is set up right now.
            if (nr >= 0 && nr < curMap.getWorldHeight_gridUnits()
                && nc >= 0 && nc < curMap.getWorldWidth_gridUnits()
                && (!(curMap.intGridType(nc * player.get_singleGU_sideLen_inPixels(), nr * player.get_singleGU_sideLen_inPixels()) == 1))
                && (!(curMap.intGridType(nc * player.get_singleGU_sideLen_inPixels(), nr * player.get_singleGU_sideLen_inPixels()) == 2))
                && !visited.contains({ nr, nc })) {

                costMap[nr][nc] = curDist + 1;
                q.push({ nr, nc, curDist + 1 });
                visited.insert({ nr, nc });
            }
        }
    }
    return costMap;
}



// Vector Field for the entire map, utilizing the costMap as the source for target goal cell.
std::vector<std::vector<std::pair<int, int>>> CreateVectorMap(Map& curMap, std::vector<std::vector<int>>& costMap) {

    // 8 Directions 
    constexpr std::array<std::array<int, 2>, 8> dirs{ {{0, 1}, {0, -1}, {1, 0}, {-1, 0},
                                                       {1, 1}, {1, -1}, {-1, 1}, {-1, -1}}
    };
    const int h_gUnits = curMap.getWorldHeight_gridUnits();
    const int w_gUnits = curMap.getWorldWidth_gridUnits();

    std::vector<std::vector<std::pair<int, int>>> vectorMap(
        h_gUnits,
        std::vector<std::pair<int, int>>(w_gUnits, { std::numeric_limits<int>::max(), std::numeric_limits<int>::max() })
    );

    for (int curR = 0; curR < curMap.getWorldHeight_gridUnits(); ++curR) {
        for (int curC = 0; curC < curMap.getWorldWidth_gridUnits(); ++curC) {

            // The current cell is unreachable, so just continue.
            if (costMap[curR][curC] == -1) {
                continue;
            }

            int bestCost = std::numeric_limits<int>::max();
            std::pair<int, int> bestDir = { std::numeric_limits<int>::max(), std::numeric_limits<int>::max() };

            for (auto [dr, dc] : dirs) {
                int nr = curR + dr;
                int nc = curC + dc;

                if (nr >= 0 && nr < curMap.getWorldHeight_gridUnits()
                    && nc >= 0 && nc < curMap.getWorldWidth_gridUnits()
                    && !(costMap[nr][nc] == -1)) {

                    if (costMap[nr][nc] < bestCost) {
                        bestCost = costMap[nr][nc];
                        bestDir = { dr, dc };
                    }
                    if (costMap[nr][nc] == bestCost && SDL_GetTicks() % 2 == 0) { // added for a little movement variety/randomness
                        bestCost = costMap[nr][nc];
                        bestDir = { dr, dc };
                    }
                }
            }
            vectorMap[curR][curC] = bestDir;
        }
    }

    return vectorMap;

}


void smartFollow(GameObject* hunter, Map& curMap, std::vector<std::vector<std::pair<int, int>>>& vecMap, std::vector<std::vector<int>>& costMap) {

    const int y_gu = hunter->get_y_gu();
    const int x_gu = hunter->get_x_gu();


    // Checks
    if (y_gu < 0 || y_gu >= static_cast<int>(vecMap.size())) {
        SDL_Log("SmartFollow: y grid unit out of bounds (%d)", y_gu);
        return;
    };
    if (x_gu < 0 || x_gu >= static_cast<int>(vecMap[y_gu].size())) {
        SDL_Log("SmartFollow: x grid unit out of bounds (%d)", x_gu);
        return;
    };

    auto [dy_int, dx_int] = vecMap[y_gu][x_gu];

    // SAFETY: IF the hunter clips a corner, it can get weird. So this code allievates that problem. 
    if (dy_int == std::numeric_limits<int>::max() && dx_int == std::numeric_limits<int>::max()) {
        #if DEBUG_GAME
            SDL_Log("SmartFollow: direction unset {INT_MAX, INT_MAX}. Weird corner clip case. Enemy is redirected.");
        #endif
        constexpr std::array<std::array<int, 2>, 4> dirs{ {{0, 1}, {0, -1}, {1, 0}, {-1, 0}} };
        std::unordered_set<std::pair<int, int>, PairHash> visited;
        visited.insert({ dy_int, dx_int });
        int bestCost = std::numeric_limits<int>::max();
        std::pair<int, int> bestDir = { std::numeric_limits<int>::max(), std::numeric_limits<int>::max() };

        for (auto [dr, dc] : dirs) {
            int nr = y_gu + dr;
            int nc = x_gu + dc;

            if (nr >= 0 && nr < curMap.getWorldHeight_gridUnits()
                && nc >= 0 && nc < curMap.getWorldWidth_gridUnits()
                && !(costMap[nr][nc] == -1) && !visited.contains({ nr, nc })) {

                if (costMap[nr][nc] < bestCost) {
                    bestCost = costMap[nr][nc];
                    bestDir = { dr, dc };
                }
                if (costMap[nr][nc] == bestCost && SDL_GetTicks() % 2 == 0) { // added 
                    bestCost = costMap[nr][nc];
                    bestDir = { dr, dc };
                }
            }
            else {
                visited.insert({ nr, nc });
            }
        }

        dy_int = bestDir.first;
        dx_int = bestDir.second;
        
        if (dy_int == std::numeric_limits<int>::max() || dx_int == std::numeric_limits<int>::max()) {
            #if DEBUG_GAME
                SDL_Log("SmartFollow: direction unset {INT_MAX, INT_MAX}. Redirect Failed");
            #endif
            return;
        }
    }

    float dy = static_cast<float>(dy_int);
    float dx = static_cast<float>(dx_int);

    if (dy != 0.0f && dx != 0.0f) {
        dy *= 0.70710678f;
        dx *= 0.70710678f;
    }

    const double ai_dt = 1.0 / 60.0;

    hunter->set_x_px(hunter->get_x_px() + dx * hunter->get_speed_pixels() * (float)ai_dt);
    hunter->set_y_px(hunter->get_y_px() + dy * hunter->get_speed_pixels() * (float)ai_dt);
    hunter->set_x_gu(static_cast<int>(std::floor(hunter->get_x_px() / hunter->get_singleGU_sideLen_inPixels())));
    hunter->set_y_gu(static_cast<int>(std::floor(hunter->get_y_px() / hunter->get_singleGU_sideLen_inPixels())));


}



void RenderArrows(SDL_Renderer* r, Camera2D& cam, std::vector<SDL_Texture*>& arrowTextures, Map& curMap, float tileSize_pixels, std::vector<std::vector<std::pair<int, int>>>& vecMap) {

    // 8 Directions                                       
    constexpr std::array<std::pair<int, int>, 8> dirs{ {
        {-1, 0},  // N
        {-1, 1},  // NE        
        {0, 1},   // E
        {1, 1},   // SE
        {1, 0},   // S
        {1, -1},  // SW
        {0, -1},  // W
        {-1, -1}} // NW
    };

    int x0 = (int)std::floor(cam.x / tileSize_pixels);
    int y0 = (int)std::floor(cam.y / tileSize_pixels);

    int x1 = (int)std::floor((cam.x + cam.w) / tileSize_pixels);
    int y1 = (int)std::floor((cam.y + cam.h) / tileSize_pixels);

    // pad by 1 tile so edges don’t pop
    x0 -= 1; y0 -= 1;
    x1 += 1; y1 += 1;

    if (vecMap.empty() || vecMap[0].empty()) return;

    int mapH = (int)vecMap.size();
    int mapW = (int)vecMap[0].size();
    x0 = std::max(0, x0);
    y0 = std::max(0, y0);
    x1 = std::min(mapW - 1, x1);
    y1 = std::min(mapH - 1, y1);

    for (int ty = y0; ty <= y1; ++ty) {
        for (int tx = x0; tx <= x1; ++tx) {

            std::pair<int, int> curDir = vecMap[ty][tx];
            if (curDir.first == std::numeric_limits<int>::max()) continue;

            for (int i = 0; i < dirs.size(); ++i) {

                if (curDir == dirs[i]) {
                    SDL_Texture* tex = arrowTextures[i];

                    SDL_FRect dst{
                        tx * tileSize_pixels - cam.x,   // convert world -> screen by subtracting camera
                        ty * tileSize_pixels - cam.y,
                        (float)tileSize_pixels,
                        (float)tileSize_pixels
                    };
                    SDL_RenderTexture(r, tex, nullptr, &dst);
                    continue;

                }
            }
        }
    }
}


void followTargetSimple(GameObject& hunter, GameObject& target, TileMap& curMap, float speed, const std::vector<TileDef>& catalog) {
    float xDiff = target.get_x_px() - hunter.get_x_px();
    float yDiff = target.get_y_px() - hunter.get_y_px();

    float followSpeed = speed;
    float dt = 1.0 / 60.0; // events/second aka hz
    float distSq = (xDiff * xDiff) + (yDiff * yDiff);

    // In zone -> move toward player
    if (distSq < 0.000001f) return; // already at same point

    float dist = std::sqrt(distSq);
    float invDist = 1.0f / dist;

    float dirX = xDiff * invDist;
    float dirY = yDiff * invDist;

    float oldX = hunter.get_x_px();
    float oldY = hunter.get_y_px();

    hunter.set_x_px(hunter.get_x_px() + (dirX * followSpeed * dt));
    hunter.set_y_px(hunter.get_y_px() + (dirY * followSpeed * dt));

    if (curMap.IsSolidAt(hunter.get_x_px(), hunter.get_y_px(), catalog)) {
        hunter.set_x_px(oldX);
        hunter.set_y_px(oldY);
    }
    else if (curMap.IsSolidAt(hunter.get_x_px() + 8, hunter.get_y_px(), catalog)) {
        hunter.set_x_px(oldX);
        hunter.set_y_px(oldY);
    }
    else if (curMap.IsSolidAt(hunter.get_x_px(), hunter.get_y_px() + 36, catalog)) {
        hunter.set_x_px(oldX);
        hunter.set_y_px(oldY);
    }
    else if (curMap.IsSolidAt(hunter.get_x_px() + 8, hunter.get_y_px() + 36,catalog)) {
        hunter.set_x_px(oldX);
        hunter.set_y_px(oldY);
    }

    hunter.set_x_gu(static_cast<int>(std::floor(hunter.get_x_px() / hunter.get_singleGU_sideLen_inPixels())));
    hunter.set_y_gu(static_cast<int>(std::floor(hunter.get_y_px() / hunter.get_singleGU_sideLen_inPixels())));

}


std::vector<std::vector<int>> CreateCostMap(TileMap& curMap, GameObject& player, const std::vector<TileDef>& catalog) {

    // 8 directions
    constexpr std::array<std::array<int, 2>, 8> dirs{ {{0, 1}, {0, -1}, {1, 0}, {-1, 0},
                                                       {1, 1}, {1, -1}, {-1, 1}, {-1, -1}}
    };

    const int h_gUnits = curMap.getWorldHeight_gridUnits(); // rows
    const int w_gUnits = curMap.getWorldWidth_gridUnits();  // cols

    std::vector<std::vector<int>> costMap( h_gUnits,
                                           std::vector<int>(w_gUnits, -1)
    );

    // Potential Optimization: replace q with another Data Structure
    std::queue<std::array<int, 3>> q;
    q.push({ player.get_y_gu(), player.get_x_gu(), 0 });

    std::unordered_set<std::pair<int, int>, PairHash> visited;
    costMap[player.get_y_gu()][player.get_x_gu()] = 0;
    visited.insert({ player.get_y_gu(), player.get_x_gu() });

    while (!q.empty()) {

        auto [curR, curC, curDist] = q.front();
        q.pop();

        for (const auto& [dr, dc] : dirs) {
            int nr = curR + dr;
            int nc = curC + dc;
            if (nr >= 0 && nr < curMap.getWorldHeight_gridUnits()
                && nc >= 0 && nc < curMap.getWorldWidth_gridUnits()
                && !curMap.IsSolidAt(nc * player.get_singleGU_sideLen_inPixels(), nr * player.get_singleGU_sideLen_inPixels(), catalog)
                && !visited.contains({ nr, nc })) {

                costMap[nr][nc] = curDist + 1;
                q.push({ nr, nc, curDist + 1 });
                visited.insert({ nr, nc });
            }
        }
    }
    return costMap;
}



// Vector Field for the entire map, utilizing the costMap as the source for target goal cell.
std::vector<std::vector<std::pair<int, int>>> CreateVectorMap(TileMap& curMap, std::vector<std::vector<int>>& costMap) {
    
    // 8 Directions 
    constexpr std::array<std::array<int, 2>, 8> dirs{ {{0, 1}, {0, -1}, {1, 0}, {-1, 0},
                                                       {1, 1}, {1, -1}, {-1, 1}, {-1, -1}}
    };
    const int h_gUnits = curMap.getWorldHeight_gridUnits();
    const int w_gUnits = curMap.getWorldWidth_gridUnits();

    std::vector<std::vector<std::pair<int, int>>> vectorMap(
        h_gUnits,
        std::vector<std::pair<int, int>>(w_gUnits, { std::numeric_limits<int>::max(), std::numeric_limits<int>::max() })
    );

    for (int curR = 0; curR < curMap.getWorldHeight_gridUnits(); ++curR) {
        for (int curC = 0; curC < curMap.getWorldWidth_gridUnits(); ++curC) {
            
            // The current cell is unreachable, so just continue.
            if (costMap[curR][curC] == -1) {
                continue;
            }

            int bestCost = std::numeric_limits<int>::max();
            std::pair<int, int> bestDir = { std::numeric_limits<int>::max(), std::numeric_limits<int>::max() };
            
            for (auto [dr, dc] : dirs) {
                int nr = curR + dr;
                int nc = curC + dc;

                if (nr >= 0 && nr < curMap.getWorldHeight_gridUnits()
                    && nc >= 0 && nc < curMap.getWorldWidth_gridUnits()
                    && !(costMap[nr][nc] == -1)) {

                    if (costMap[nr][nc] < bestCost) {
                        bestCost = costMap[nr][nc];
                        bestDir = { dr, dc };
                    }
                    if (costMap[nr][nc] == bestCost && SDL_GetTicks() % 2 == 0) { // added for a little movement variety/randomness
                        bestCost = costMap[nr][nc];
                        bestDir = { dr, dc };
                    }
                }
            }
            vectorMap[curR][curC] = bestDir;
        }
    }

    return vectorMap;

}


void smartFollow(GameObject* hunter, TileMap& curMap, std::vector<std::vector<std::pair<int, int>>>& vecMap, std::vector<std::vector<int>>& costMap) {

    const int y_gu = hunter->get_y_gu();
    const int x_gu = hunter->get_x_gu();


    // Checks
    if (y_gu < 0 || y_gu >= static_cast<int>(vecMap.size())) {
        SDL_Log("SmartFollow: y grid unit out of bounds (%d)", y_gu);
        return;
    };
    if (x_gu < 0 || x_gu >= static_cast<int>(vecMap[y_gu].size())) {
        SDL_Log("SmartFollow: x grid unit out of bounds (%d)", x_gu);
        return;
    };

    auto [dy_int, dx_int] = vecMap[y_gu][x_gu];

    // SAFETY: IF the hunter clips a corner, it can get weird. So this code allievates that problem. 
    if (dy_int == std::numeric_limits<int>::max() && dx_int == std::numeric_limits<int>::max()) {
        #if DEBUG_GAME
            SDL_Log("SmartFollow: direction unset {INT_MAX, INT_MAX}. Weird corner clip case. Enemy is redirected.");
        #endif
        constexpr std::array<std::array<int, 2>, 4> dirs{ {{0, 1}, {0, -1}, {1, 0}, {-1, 0}}};
        std::unordered_set<std::pair<int, int>, PairHash> visited;
        visited.insert({ dy_int, dx_int });
        int bestCost = std::numeric_limits<int>::max();
        std::pair<int, int> bestDir = { std::numeric_limits<int>::max(), std::numeric_limits<int>::max() };

        for (auto [dr, dc] : dirs) {
            int nr = y_gu + dr;
            int nc = x_gu + dc;

            if (nr >= 0 && nr < curMap.getWorldHeight_gridUnits()
                && nc >= 0 && nc < curMap.getWorldWidth_gridUnits()
                && !(costMap[nr][nc] == -1) && !visited.contains({ nr, nc })) {

                if (costMap[nr][nc] < bestCost) {
                    bestCost = costMap[nr][nc];
                    bestDir = { dr, dc };
                }
                if (costMap[nr][nc] == bestCost && SDL_GetTicks() % 2 == 0) { // added 
                    bestCost = costMap[nr][nc];
                    bestDir = { dr, dc };
                }
            }
            else {
                visited.insert({ nr, nc });
            }
        }

        dy_int = bestDir.first;
        dx_int = bestDir.second;
        
        if (dy_int == std::numeric_limits<int>::max() || dx_int == std::numeric_limits<int>::max()) {
            #if DEBUG_GAME
                SDL_Log("SmartFollow: direction unset {INT_MAX, INT_MAX}. Redirect Failed");
            #endif
            return;
        }
        
    }


    float dy = static_cast<float>(dy_int);
    float dx = static_cast<float>(dx_int);

    if (dy != 0.0f && dx != 0.0f) {
        dy *= 0.70710678f;
        dx *= 0.70710678f;
    }

    const double ai_dt = 1.0 / 60.0;

    hunter->set_x_px(hunter->get_x_px() + dx * hunter->get_speed_pixels() * (float)ai_dt);
    hunter->set_y_px(hunter->get_y_px() + dy * hunter->get_speed_pixels() * (float)ai_dt );
    hunter->set_x_gu(static_cast<int>(std::floor(hunter->get_x_px() / hunter->get_singleGU_sideLen_inPixels())));
    hunter->set_y_gu(static_cast<int>(std::floor(hunter->get_y_px() / hunter->get_singleGU_sideLen_inPixels())));


}






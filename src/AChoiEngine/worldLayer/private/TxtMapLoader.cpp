#include "AChoiEngine/worldLayer/public/TxtMapLoader.h"
#include <SDL3_image/SDL_image.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


bool TileMap::LoadLevelFromTxt(const char* kLevelPath,
    int& ref_worldWidth_gridUnits,
    int& ref_worldHeight_gridUnits,
    std::vector<int>& outTiles) {

    std::ifstream file(kLevelPath);
    if (!file.is_open()) return false;

    outTiles.clear();
    ref_worldWidth_gridUnits = 0;
    ref_worldHeight_gridUnits = 0;

    std::string curLine;
    while (std::getline(file, curLine)) {
        if (curLine.empty()) continue;

        std::stringstream ss(curLine);
        int value;
        int curWidth = 0;

        while (ss >> value) {  // extract the next integer from the stream, store it in value
            outTiles.push_back(value);
            curWidth++;
        }

        if (ref_worldWidth_gridUnits == 0) ref_worldWidth_gridUnits = curWidth;
        else if (ref_worldWidth_gridUnits != curWidth) return false;  // inconsistent row length

        ref_worldHeight_gridUnits++;
    }

    /* worldWidth_gridUnits_ = dref_worldWidth_gridUnits;
     worldHeight_gridUnits_ = dref_worldHeight_gridUnits;*/

    worldWidth_pixels_ = static_cast<float>(ref_worldWidth_gridUnits) * tileSize_pixels_;
    worldHeight_pixels_ = static_cast<float>(ref_worldHeight_gridUnits) * tileSize_pixels_;

    return ref_worldWidth_gridUnits > 0 && ref_worldHeight_gridUnits > 0;
}

void TileMap::Shutdown() {
    for (SDL_Texture* tex : tilesets_) {
        if (tex) SDL_DestroyTexture(tex);
    }
    tilesets_.clear();
    tileIDs_.clear();
    worldWidth_gridUnits_ = worldHeight_gridUnits_ = 0;
}

bool TileMap::LoadTiles(SDL_Renderer* appR,
    const std::vector<std::string>& tilesetPaths,
    const char* animPath,
    const char* kLevelPath,
    float tileSizePixels) {

    Shutdown();
    tileSize_pixels_ = tileSizePixels;

    // Load all tilesheets
    tilesets_.reserve(tilesetPaths.size());
    for (const std::string& curTilePath : tilesetPaths) {

        SDL_Surface* surf = IMG_Load(curTilePath.c_str());
        if (!surf) {
            SDL_Log("BOOOOO (%s): %s", "tiles as surface failed to load", SDL_GetError());
            Shutdown();
            return false;
        }
        SDL_Texture* tex = SDL_CreateTextureFromSurface(appR, surf);
        if (!tex) {
            SDL_Log("BOOOOO (%s): %s", "Tiles as texture failed to load", SDL_GetError());
            Shutdown();
            return false;
        }
        SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
        SDL_DestroySurface(surf);
        tilesets_.push_back(tex);
    }

    if (!LoadLevelFromTxt(kLevelPath, worldWidth_gridUnits_, worldHeight_gridUnits_, tileIDs_)) {
        SDL_Log("BOOOOO (%s): %s", "txt failed to load", SDL_GetError());
        Shutdown();
        return false;
    }

    return true;
}

void TileMap::Render(SDL_Renderer* appR, Camera2D& cam, const std::vector<TileDef>& catalog) const {
    

    int x0 = (int)std::floor(cam.x / tileSize_pixels_);
    int y0 = (int)std::floor(cam.y / tileSize_pixels_);

    int x1 = (int)std::floor((cam.x + cam.w) / tileSize_pixels_);
    int y1 = (int)std::floor((cam.y + cam.h) / tileSize_pixels_);

    // pad by 1 tile so edges don’t pop
    x0 -= 1; y0 -= 1;
    x1 += 1; y1 += 1;

    x0 = std::max(0, x0);
    y0 = std::max(0, y0);
    x1 = std::min(worldWidth_gridUnits_ - 1, x1);
    y1 = std::min(worldHeight_gridUnits_ - 1, y1);

    for (int ty = y0; ty <= y1; ++ty) {
        for (int tx = x0; tx <= x1; ++tx) {

            int curTileID = tileIDs_[ty * worldWidth_gridUnits_ + tx];
            if (curTileID < 0 || curTileID >= (int)catalog.size()) continue;
            const TileDef& curTile = catalog[curTileID];
            SDL_Texture* tex = tilesets_[curTile.sheetIndex];


            const SDL_FRect src{
                curTile.sheetX * tileSize_pixels_,
                curTile.sheetY * tileSize_pixels_,
                tileSize_pixels_,
                tileSize_pixels_
            };

            SDL_FRect dst{
                tx * tileSize_pixels_ - cam.x,   // convert world -> screen by subtracting camera
                ty * tileSize_pixels_ - cam.y,
                (float)tileSize_pixels_,
                (float)tileSize_pixels_
            };

            SDL_RenderTexture(appR, tex, &src, &dst);
        }
    }

}

bool TileMap::IsSolidAt(float worldX, float worldY, const std::vector<TileDef>& catalog) const {
    //const auto& catalog = GetTileCatalog();

    int tx = (int)(worldX / tileSize_pixels_);
    int ty = (int)(worldY / tileSize_pixels_);

    if (tx < 0 || tx >= worldWidth_gridUnits_ || ty < 0 || ty >= worldHeight_gridUnits_)
        return true;

    int curTileID = tileIDs_[ty * worldWidth_gridUnits_ + tx];
    if (curTileID < 0 || curTileID >= (int)catalog.size())
        return true;

    return catalog[curTileID].isSolid;
}

bool TileMap::DoesDamageAt(float worldX, float worldY, const std::vector<TileDef>& catalog) const {
    //const auto& catalog = GetTileCatalog();

    int tx = (int)(worldX / tileSize_pixels_);
    int ty = (int)(worldY / tileSize_pixels_);

    if (tx < 0 || tx >= worldWidth_gridUnits_ || ty < 0 || ty >= worldHeight_gridUnits_)
        return false;

    int curTileID = tileIDs_[ty * worldWidth_gridUnits_ + tx];
    if (curTileID < 0 || curTileID >= (int)catalog.size())
        return false;

    return catalog[curTileID].doesDamage;
}



int TileMap::getWorldHeight_gridUnits() {
    return worldHeight_gridUnits_;
}
int TileMap::getWorldWidth_gridUnits() {
    return worldWidth_gridUnits_;
}

float TileMap::getWorldHeight_pixels() {
    return worldHeight_pixels_;
}
float TileMap::getWorldWidth_pixels() {
    return worldWidth_pixels_;
}
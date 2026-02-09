#include "AChoiEngine/worldLayer/public/MapObject.h"
#include "AChoiEngine/worldLayer/public/LdtkMapLoader.h"
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <filesystem>
#include <nlohmann/json.hpp>
#include "AChoiEngine/camera/public/Camera.h"
#include "assets.h"


bool ACE_MapObject::ACE_Init(SDL_Renderer* appR, const char* jsonPath) {
    
    ldtk_project = ACE_LoadProject(jsonPath);

    ACE_Shutdown();

    tileSize_pixels_ = ldtk_project.layerInstances[1].gridSize;
    worldWidth_gridUnits_ = ldtk_project.layerInstances[1].world_gUnits_width;
    worldHeight_gridUnits_ = ldtk_project.layerInstances[1].world_gUnits_height;
    worldWidth_pixels_ = static_cast<float>(worldWidth_gridUnits_) * tileSize_pixels_;
    worldHeight_pixels_ = static_cast<float>(worldHeight_gridUnits_) * tileSize_pixels_;

    // Load all tilesheets
    for (const auto& layer : ldtk_project.layerInstances) {
        if (layer.l_type == LDTK_LayerType::Tiles) {

            std::filesystem::path ldtkPath = jsonLevel1.c_str();
            std::filesystem::path ldtkDir = ldtkPath.parent_path();
            std::filesystem::path imagePath = ldtkDir / layer.relPath;
            imagePath = imagePath.lexically_normal();

            SDL_Surface* surf = IMG_Load(imagePath.string().c_str());
            if (!surf) {
                SDL_Log("BOOOOO HOOO (%s): %s", "tiles as surface failed to load", SDL_GetError());
                ACE_Shutdown();
                return false;
            }
            SDL_Texture* tex = SDL_CreateTextureFromSurface(appR, surf);
            if (!tex) {
                SDL_Log("BOOOOO HOOO (%s): %s", "Tiles as texture failed to load", SDL_GetError());
                ACE_Shutdown();
                return false;
            }
            SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
            SDL_DestroySurface(surf);
            tilesets_.push_back(tex);
        }
    }

    // Kinda dangerous at the moment. I should make this more robust when I get a chance.
    solids_ = ldtk_project.project_intGrid[0].layer_intGrid;
    
    return worldWidth_gridUnits_ > 0 && worldHeight_gridUnits_ > 0;
}


void ACE_MapObject::ACE_Shutdown() {
    for (SDL_Texture* tex : tilesets_) {
        if (tex) SDL_DestroyTexture(tex);
    }
    tilesets_.clear();
    worldWidth_gridUnits_ = worldHeight_gridUnits_ = 0;
}

void ACE_MapObject::ACE_Render(SDL_Renderer* appR, ACE_Camera2D& cam) const {
    
    if (ldtk_project.layerInstances.size() != tilesets_.size()) {
        throw std::runtime_error("Vector size mismatch");
    }
    
    for (size_t iLayer = 0; iLayer < ldtk_project.layerInstances.size(); ++iLayer) {
        if (ldtk_project.layerInstances[iLayer].l_type == LDTK_LayerType::IntGrid) {
            continue;
        }

        SDL_Texture* tex = tilesets_[iLayer];

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

                int temp = ty * worldWidth_gridUnits_ + tx;
                const LDTK_levels_layerInstances_gridTiles& curGridTile
                    = ldtk_project.layerInstances[iLayer].ldtk_gridTiles[ty * worldWidth_gridUnits_ + tx];

                if (curGridTile.exists == false) {
                    continue;
                }

                const SDL_FRect src{
                    curGridTile.src_tileset_px.x_c_px,
                    curGridTile.src_tileset_px.y_r_px,
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
}

void ACE_MapObject::ACE_Render(SDL_Renderer* appR) const {

    if (ldtk_project.layerInstances.size() != tilesets_.size()) {
        throw std::runtime_error("Vector size mismatch");
    }

    for (size_t iLayer = 0; iLayer < ldtk_project.layerInstances.size(); ++iLayer) {
        if (ldtk_project.layerInstances[iLayer].l_type == LDTK_LayerType::IntGrid) {
            continue;
        }

        SDL_Texture* tex = tilesets_[iLayer];

        for (int y = 0; y < worldHeight_gridUnits_; ++y) {
            for (int x = 0; x < worldWidth_gridUnits_; ++x) {
                const LDTK_levels_layerInstances_gridTiles& curGridTile
                    = ldtk_project.layerInstances[iLayer].ldtk_gridTiles[y * worldWidth_gridUnits_ + x];
                const SDL_FRect src{
                        curGridTile.src_tileset_px.x_c_px,
                        curGridTile.src_tileset_px.y_r_px,
                        tileSize_pixels_,
                        tileSize_pixels_
                };

                SDL_FRect dst{
                    x * tileSize_pixels_,
                    y * tileSize_pixels_,
                    (float)tileSize_pixels_,
                    (float)tileSize_pixels_
                };

                // Before rendering (or whenever you want to change opacity)
                SDL_SetTextureAlphaMod(tex, 128);

                SDL_RenderTexture(appR, tex, &src, &dst);
            }
        }
    }
}

void ACE_MapObject::ACE_Render(SDL_Renderer* appR, int alpha) const {

    if (ldtk_project.layerInstances.size() != tilesets_.size()) {
        throw std::runtime_error("Vector size mismatch");
    }
    
    for (size_t iLayer = 0; iLayer < ldtk_project.layerInstances.size(); ++iLayer) {
        if (ldtk_project.layerInstances[iLayer].l_type == LDTK_LayerType::IntGrid) {
            continue;
        }

        SDL_Texture* tex = tilesets_[iLayer];

        for (int y = 0; y < worldHeight_gridUnits_; ++y) {
            for (int x = 0; x < worldWidth_gridUnits_; ++x) {
                const LDTK_levels_layerInstances_gridTiles& curGridTile
                    = ldtk_project.layerInstances[iLayer].ldtk_gridTiles[y * worldWidth_gridUnits_ + x];
                const SDL_FRect src{
                        curGridTile.src_tileset_px.x_c_px,
                        curGridTile.src_tileset_px.y_r_px,
                        tileSize_pixels_,
                        tileSize_pixels_
                };

                SDL_FRect dst{
                    x * tileSize_pixels_,
                    y * tileSize_pixels_,
                    (float)tileSize_pixels_,
                    (float)tileSize_pixels_
                };

                SDL_SetTextureAlphaMod(tex, alpha);
                SDL_RenderTexture(appR, tex, &src, &dst);
            }
        }
    }
}



// returns the int in the IntGridCSV at the specific coordinates
int ACE_MapObject::ACE_intGridType(float worldX_pixel, float worldY_pixel) const {

    int tx = static_cast<int>(worldX_pixel / tileSize_pixels_);
    int ty = static_cast<int>(worldY_pixel / tileSize_pixels_);
    if (tx < 0 || tx >= worldWidth_gridUnits_ || ty < 0 || ty >= worldHeight_gridUnits_)
        return true;

    int idx = ty * worldWidth_gridUnits_ + tx;
    if (idx < 0 || idx >= (int)solids_.size()) return true;

    return solids_[idx];

}

int ACE_MapObject::getWorldHeight_gridUnits() const { return worldHeight_gridUnits_; }
int ACE_MapObject::getWorldWidth_gridUnits() const { return worldWidth_gridUnits_; }
float ACE_MapObject::getWorldHeight_pixels() const { return worldHeight_pixels_; }
float ACE_MapObject::getWorldWidth_pixels() const { return worldWidth_pixels_; }
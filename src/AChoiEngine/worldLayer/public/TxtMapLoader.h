#pragma once
#include <vector>
#include <string>
#include "AChoiEngine/camera/public/Camera.h"

class ACE_Camera2D;

struct ACE_TileDef {
    int sheetIndex;
    float sheetX;
    float sheetY;
    bool isSolid;
    bool doesDamage;
};


class ACE_TileMap {
public:
	bool ACE_LoadTiles(SDL_Renderer* appR,
		const std::vector<std::string>& tilesetPaths,
		const char* animPath,
		const char* kLevelPath,
		float tileSizePixels);

	bool ACE_LoadLevelFromTxt(const char* kLevelPath,
		int& worldWidth_gridUnits,
		int& worldHeight_gridUnits,
		std::vector<int>& outTiles);
	void ACE_Shutdown();
	void ACE_Render(SDL_Renderer* appR, ACE_Camera2D& cam, const std::vector<ACE_TileDef>& catalog) const;
	bool ACE_IsSolidAt(float worldX_pixel, float worldY_pixel, const std::vector<ACE_TileDef>& catalog) const;
	bool ACE_DoesDamageAt(float worldX_pixel, float worldY_pixel, const std::vector<ACE_TileDef>& catalog) const;

	int getWorldHeight_gridUnits();
	int getWorldWidth_gridUnits();

	float getWorldHeight_pixels();
	float getWorldWidth_pixels();



private:
	float tileSize_pixels_ = 0.0f;
	int worldWidth_gridUnits_ = 0;
	int worldHeight_gridUnits_ = 0;

	float worldWidth_pixels_ = 0.0f;
	float worldHeight_pixels_ = 0.0f;

	std::vector<int> tileIDs_;
	std::vector<SDL_Texture*> tilesets_;  // Vector of SDL_Textures. Each texture is one of the images of 
	// the environment in the assets/textures folder.
};
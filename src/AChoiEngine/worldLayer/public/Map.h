#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <string>
#include "AChoiEngine/worldLayer/public/LdtkMapLoader.h"

class ACE_Camera2D;

class ACE_Map {
public:

	bool ACE_Init(SDL_Renderer* appR, const char* jsonPath);
	void ACE_Shutdown();
	void ACE_Render(SDL_Renderer* appR, ACE_Camera2D& cam) const;
	void ACE_Render(SDL_Renderer* appR) const;
	void ACE_Render(SDL_Renderer* appR, int alpha) const;


	int ACE_intGridType(float worldX_pixel, float worldY_pixel) const;

	int getWorldHeight_gridUnits() const;
	int getWorldWidth_gridUnits() const;
	float getWorldHeight_pixels() const;
	float getWorldWidth_pixels() const;

private:
	float tileSize_pixels_ = 0.0f;
	int worldWidth_gridUnits_ = 0;
	int worldHeight_gridUnits_ = 0;

	float worldWidth_pixels_ = 0.0f;
	float worldHeight_pixels_ = 0.0f;

	std::vector<SDL_Texture*> tilesets_;  // Vector of SDL_Textures. Each texture is one of the images of 

	LDTK_Project ldtk_project;
	std::vector<int> solids_;
};
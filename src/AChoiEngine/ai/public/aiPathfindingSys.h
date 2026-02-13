#pragma once
#include <vector>
#include "AChoiEngine/worldLayer/public/TxtMapLoader.h"

class ACE_GameObject;
class ACE_MapObject;
class ACE_Camera2D;
class SDL_Renderer;
class SDL_Texture;

// Takes in Map parameter (for maps loaded in via LDTK)
void ACE_followTargetSimple(ACE_GameObject& hunter, ACE_GameObject& target, ACE_MapObject& curMap, float speed);

void ACE_smartFollow_aStar(ACE_GameObject& hunter, ACE_GameObject& target, ACE_MapObject& curMap, float speed);

void ACE_smartFollow_vecField(ACE_GameObject* hunter,
							  ACE_MapObject& curMap,
							  std::vector<std::vector<std::pair<int, int>>>& vecMap,
							  std::vector<std::vector<int>>& costMap);

std::vector<std::vector<int>> ACE_createCostMap(ACE_MapObject& curMap, ACE_GameObject& player);

std::vector<std::vector<std::pair<int, int>>> ACE_createVectorMap(ACE_MapObject& curMap, std::vector<std::vector<int>>& costMap);

void ACE_renderArrows(SDL_Renderer* r,
					  ACE_Camera2D& cam,
					  std::vector<SDL_Texture*>& arrowTextures,
					  ACE_MapObject& curMap,
					  float tileSize_pixels,
					  std::vector<std::vector<std::pair<int, int>>>& vecMap);




// Takes in TileMap parameter (for maps loaded in via txt)
// Will probably no longer update, but will keep this as legacy code.
void ACE_followTargetSimple(ACE_GameObject& hunter, ACE_GameObject& target, ACE_TileMap& curMap, float speed, const std::vector<ACE_TileDef>& catalog);
void ACE_smartFollow(ACE_GameObject* hunter, ACE_TileMap& curMap, std::vector<std::vector<std::pair<int, int>>>& vecMap, std::vector<std::vector<int>>& costMap);
std::vector<std::vector<int>> ACE_CreateCostMap(ACE_TileMap& curMap, ACE_GameObject& player, const std::vector<ACE_TileDef>& catalog);
std::vector<std::vector<std::pair<int, int>>> ACE_CreateVectorMap(ACE_TileMap& curMap, std::vector<std::vector<int>>& costMap);




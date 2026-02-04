#pragma once
#include <vector>
#include "AChoiEngine/worldLayer/public/TxtMapLoader.h"

class GameObject;
class Map;
class Camera2D;
class SDL_Renderer;
class SDL_Texture;


// Takes in Map parameter (for maps loaded in via LDTK)
void followTargetSimple(GameObject& hunter, GameObject& target, Map& curMap, float speed);
void smartFollow(GameObject* hunter,
				 Map& curMap,
				 std::vector<std::vector<std::pair<int, int>>>& vecMap,
				 std::vector<std::vector<int>>& costMap);
std::vector<std::vector<int>> CreateCostMap(Map& curMap, GameObject& player);
std::vector<std::vector<std::pair<int, int>>> CreateVectorMap(Map& curMap, std::vector<std::vector<int>>& costMap);
void RenderArrows(SDL_Renderer* r,
				  Camera2D& cam,
				  std::vector<SDL_Texture*>& arrowTextures,
				  Map& curMap,
				  float tileSize_pixels,
				  std::vector<std::vector<std::pair<int, int>>>& vecMap);


// Takes in TileMap parameter (for maps loaded in via txt)
void followTargetSimple(GameObject& hunter, GameObject& target, TileMap& curMap, float speed);
void smartFollow(GameObject* hunter, 
				 TileMap& curMap, 
				 std::vector<std::vector<std::pair<int, int>>>& vecMap,
				 std::vector<std::vector<int>>& costMap);
std::vector<std::vector<int>> CreateCostMap(TileMap& curMap, GameObject& player, const std::vector<TileDef>& catalog);
std::vector<std::vector<std::pair<int, int>>> CreateVectorMap(TileMap& curMap, std::vector<std::vector<int>>& costMap);




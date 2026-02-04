#pragma once
#include <vector>
#include <string>

enum class LDTK_LayerType { IntGrid, Entities, Tiles, AutoLayer, Unknown };

struct yxPixels_float {
	float x_c_px = 0;  // aka col
	float y_r_px = 0;  // aka row
};

struct yxPixels_int {
	int x_c_px = 0;  // aka col
	int y_r_px = 0;  // aka row
};

struct LDTK_TileInstance {
	//int tilesetDEF_uid = -1;	 // the TILESET this particular tile instance belongs to
	yxPixels_float src_tileset_px;  // top-left in pixels (tileset texture space)
	yxPixels_int dst_level_px;    // top-left in pixels (level space)
	//float w_px = 0;
	//float h_px = 0;
	//int tileID = -1;	// tileID
	//int layerDEF_uid = -1;
};

struct LDTK_defs_tilesets { // meta data
	int uid = -1;
	std::string user_defined_identifier;
	std::string relPath;
	int gridWidth = 0;
	int gridHeight = 0;
	int tileGridSize = 0;
	float w_px = 0;
	float h_px = 0;
};

struct LDTK_defs_layers {

};

struct LDTK_EntityInstance {

};


struct LDTK_levels_layerInstances_gridTiles {
	yxPixels_float src_tileset_px;  // top-left in pixels (tileset texture space)
	yxPixels_int dst_level_px;    // top-left in pixels (level space)
	float w_px = 0;
	float h_px = 0;
	std::string relPath;
	bool exists = false;

	int tilesetDEF_uid = -1;	 // the TILESET this particular tile instance belongs to
	int tileID = -1;	// tileID
	int layerDEF_uid = -1;
};



struct LDTK_levels_layerInstances {
	LDTK_LayerType l_type = LDTK_LayerType::Unknown;
	std::string relPath = "";
	std::vector<LDTK_levels_layerInstances_gridTiles> ldtk_gridTiles;
	std::vector<int> layer_intGrid;

	int world_gUnits_width = 0;
	int world_gUnits_height = 0;
	int gridSize = 0;
	std::string user_defined_identifier;
	std::string iid;  // ex: e32ea140-fa90-11f0-962e-0fbcfd9f84e4

	std::vector<LDTK_EntityInstance> ldtk_entities_tiles;
};

struct LDTK_levels {
	float world_x_origin_px = 0;  // top left of world
	float world_y_origin_px = 0;  // top left of world
	float world_w_pixels = 0;
	float world_h_pixels = 0;
	std::vector<LDTK_levels_layerInstances> ldtk_layerInstances;

	std::string user_defined_identifier;
	std::string iid; // ex: 45783310-fa90-11f0-962e-eb270d55f7c8
	std::string externalRelPath;
};


struct LDTK_Project {
	std::string jsonVersion;
	std::vector<LDTK_levels> ldtk_levels;
	std::vector<LDTK_levels_layerInstances> layerInstances;
	std::vector<LDTK_levels_layerInstances> project_intGrid;

};

// levels -> layerInstances -> gridTiles

LDTK_Project LoadProject(const char* ldtkPath);



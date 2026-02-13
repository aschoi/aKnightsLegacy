#include "AChoiEngine/worldLayer/public/LdtkMapLoader.h"
#include <SDL3/SDL.h>
#include <fstream>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <vector>
#include <string>

using json = nlohmann::json;

namespace {
    json LoadJsonFromFile(const char* jsonPath) {
        std::ifstream f(jsonPath);
        if (!f.is_open()) {
            throw std::runtime_error(std::string("Failed to open: ") + jsonPath);
        }
        json j;
        f >> j;
        return j;
    }
}   

// Level is not necessary safe behavior. This code assumes there's only one level.
// would like to refactor, when given the chance. 
LDTK_Project ACE_LoadProject(const char* ldtkPath) {

    LDTK_Project ldtk_project;
    
    json j_project = LoadJsonFromFile(ldtkPath);

    ldtk_project.jsonVersion = j_project.value("jsonVersion", "");

    
    json j_levels = j_project.value("levels", std::vector<json>{});
    for (const auto& j_lvl : j_levels) {
        
        LDTK_levels ldtk_lvl;
        ldtk_lvl.world_x_origin_px = j_lvl.value("worldX", 0);
        ldtk_lvl.world_y_origin_px = j_lvl.value("worldY", 0);
        ldtk_lvl.world_w_pixels = j_lvl.value("pxWid", 0);
        ldtk_lvl.world_h_pixels = j_lvl.value("pxHei", 0);

        json j_layerInstances = j_lvl.value("layerInstances", std::vector<json>{});
        for (const auto& j_layer : j_layerInstances) {
            
            if (j_layer.value("__type", "") == "Tiles") {

                LDTK_levels_layerInstances ldtk_layer;

                ldtk_layer.l_type = LDTK_LayerType::Tiles;
                ldtk_layer.relPath = j_layer.value("__tilesetRelPath", "");
                ldtk_layer.gridSize = j_layer.value("__gridSize", 0);
                ldtk_layer.world_gUnits_width = j_layer.value("__cWid", 0);
                ldtk_layer.world_gUnits_height = j_layer.value("__cHei", 0);

                ldtk_layer.ldtk_gridTiles.assign(ldtk_layer.world_gUnits_width * ldtk_layer.world_gUnits_height, LDTK_levels_layerInstances_gridTiles{});


                const auto& j_gridTiles = j_layer["gridTiles"];

                for (const auto& j_gTile : j_gridTiles) {

                    float srcX = j_gTile["src"][0].get<float>();  // x coord, aka col
                    float srcY = j_gTile["src"][1].get<float>();   // y coord, aka row

                    int dstX = static_cast<int>(j_gTile["px"][0].get<int>());
                    int dstY = static_cast<int>(j_gTile["px"][1].get<int>());

                    dstX = dstX / ldtk_layer.gridSize;
                    dstY = dstY / ldtk_layer.gridSize;

                    yxPixels_float srcXY{
                        srcX,
                        srcY
                    };
                    
                    yxPixels_int dstXY{
                       dstX,
                       dstY
                    };

                    ldtk_layer.ldtk_gridTiles[dstY * ldtk_layer.world_gUnits_width + dstX].src_tileset_px = srcXY;
                    ldtk_layer.ldtk_gridTiles[dstY * ldtk_layer.world_gUnits_width + dstX].dst_level_px = dstXY;
                    ldtk_layer.ldtk_gridTiles[dstY * ldtk_layer.world_gUnits_width + dstX].exists = true;
                }
                
                ldtk_project.layerInstances.push_back(ldtk_layer);
            }
            else if (j_layer.value("__type", "") == "IntGrid") {

                LDTK_levels_layerInstances intGrid;

                intGrid.l_type = LDTK_LayerType::IntGrid;
                intGrid.gridSize = j_layer.value("__gridSize", 0);
                intGrid.world_gUnits_width = j_layer.value("__cWid", 0);
                intGrid.world_gUnits_height = j_layer.value("__cHei", 0);
                
                const auto& j_csv = j_layer["intGridCsv"];

                for (const auto& intVal : j_csv) {
                    intGrid.layer_intGrid.push_back(intVal.get<int>());
                }
                ldtk_project.project_intGrid.push_back(intGrid);
            }
        }

        ldtk_project.ldtk_levels.push_back(ldtk_lvl);

    }
    return ldtk_project;
}






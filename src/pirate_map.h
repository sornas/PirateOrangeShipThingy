#pragma once
#include "fog.h"
#include <vector>

enum MapTile {
    UNDISCOVERED,
    WATER_TILE,
    GRASS_TILE,
};

struct PirateMap {
    std::vector<std::vector<MapTile>> tiles;
    AssetID image_id;

    int MAP_SIZE = 128;
    int TILE_SIZE = 3;

    PirateMap() {
        image_id = fog_asset_fetch_id("MAP_BASE");
        for (int i = 0; i < MAP_SIZE; i++) {
            std::vector<MapTile> row;
            for (int j = 0; j < MAP_SIZE; j++) {
                row.push_back(UNDISCOVERED);
            }
            tiles.push_back(row);
        }
    }
};

void draw_pirate_map(PirateMap& pirate_map);

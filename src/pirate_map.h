#pragma once
#include "fog.h"
#include "island.h"
#include <vector>

enum MapTile {
    WATER_TILE,
    GRASS_TILE,
};

struct PirateMap {
    std::vector<std::vector<MapTile>> tiles;
    std::vector<std::vector<bool>> discovered;
    AssetID image_id;
    int a = 0;

    int MAP_SIZE = 128;
    int TILE_SIZE = 3;

    PirateMap() : tiles(), discovered(), image_id() {}

    PirateMap(std::vector<Island>& islands) {
        image_id = fog_asset_fetch_id("MAP_BASE");
        for (int i = 0; i < MAP_SIZE; i++) {
            std::vector<MapTile> row;
            std::vector<bool> discovered_row;
            for (int j = 0; j < MAP_SIZE; j++) {
                row.push_back(WATER_TILE);
                discovered_row.push_back(false);
            }
            tiles.push_back(row);
            discovered.push_back(discovered_row);
        }

        for (Island& island : islands) {
            for (Tile& tile : island.tiles) {
                Vec2 position = island.position + tile.rel_position;
                if (is_within_bounds(position)) {
                    tiles[(int)position.y][(int)position.x] = GRASS_TILE;
                }
            }
        }
    }

    void discover(Vec2 position) {
        if (is_within_bounds(position)) {
            discovered[(int)position.y][(int)position.x] = true;
        }
    }

    bool is_within_bounds(Vec2 position) {
        return position.x >= 0 && position.x < MAP_SIZE &&
            position.y >= 0 && position.y < MAP_SIZE;
    }
};

void draw_pirate_map(PirateMap& pirate_map);
Vec3 color_from_map_tile(MapTile map_tile);

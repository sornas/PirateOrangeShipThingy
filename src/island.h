#pragma once
#include "fog.h"
#include <vector>

struct Tile {
    Vec2 rel_position;
    AssetID tile_id;

    Tile(s32 rel_x, s32 rel_y, AssetID tile_id) :
        rel_position(fog_V2(rel_x, rel_y)),
        tile_id(tile_id)
    {}
};

struct Island {
    Vec2 position;
    std::vector<Tile> tiles;
};

void init_assets();
void island_init(std::vector<Island>& islands);
void island_draw(Island& island);

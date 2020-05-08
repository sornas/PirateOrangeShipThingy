#pragma once
#include "fog.h"
#include <vector>

struct Tile {
    Vec2 rel_position;
    AssetID tile_id;

    Tile(Vec2 rel_position, AssetID tile_id) :
        rel_position(rel_position),
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

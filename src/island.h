#pragma once
#include "fog.h"
#include "orange_tree.h"
#include <vector>

struct OrangeTree;

struct Tile {
    Vec2 rel_position;
    Body body;

    Tile(s32 rel_x, s32 rel_y) : rel_position(fog_V2(rel_x, rel_y)) {}
};

enum IslandType {
    ISLAND_STARTER,
    ISLAND_SMALL,
    ISLAND_L,
    ISLAND_CRESENT,
    ISLAND_TREASURE,
    ISLAND_LIU,
};

struct Island {
    Vec2 position;
    std::vector<Tile> tiles;
    std::vector<OrangeTree> trees;
    IslandType type;
};

void init_assets();
void island_init(std::vector<Island>& islands);
void island_draw(Island& island);

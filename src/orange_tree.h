#pragma once
#include "fog.h"
#include "island.h"

struct Island;

struct OrangeTree {
    Vec2 position;
    int oranges;

    OrangeTree() : position(), oranges() {}

    OrangeTree(int x, int y) :
        position(fog_V2(x, y)),
        oranges(fog_random_int() % 4)
    {}

    void draw(Island& island);
};

void init_orange_tree();

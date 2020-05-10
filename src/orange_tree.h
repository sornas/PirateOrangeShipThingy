#pragma once
#include "fog.h"
#include "island.h"
#include <vector>

struct Island;

struct OrangeTree {
    Body body;
    int oranges;

    OrangeTree() : body(), oranges() {}

    OrangeTree(int x, int y);

    void draw();
};

std::vector<OrangeTree*> init_orange_trees(std::vector<Island>& islands);
void set_up_orange_body();

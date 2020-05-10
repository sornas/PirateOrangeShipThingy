#include <vector>

#include "fog.h"
#include "island.h"

struct Treasure {
    Body body;

    void draw();
};

Treasure init_treasure(std::vector<Island>& islands);

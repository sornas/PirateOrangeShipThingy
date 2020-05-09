#include "island.h"
#include "pirate_map.h"

AssetID CENTER;
AssetID BOTTOM;
AssetID BOTTOM_LEFT;
AssetID BOTTOM_RIGHT;
AssetID LEFT;
AssetID RIGHT;
AssetID TOP;
AssetID TOP_LEFT;
AssetID TOP_RIGHT;
AssetID SMALL_ISLAND;

void island_init(std::vector<Island>& islands) {

    init_assets();
    std::vector<Island> predefined_islands;

    // Starter island
    // ~~~~~
    // ~###~
    // ~###~
    // ~###~
    // ~~~~~
    predefined_islands.push_back({
        fog_V2(0, 0), {
            Tile(-1,  1), Tile(0,  1), Tile(1,  1),
            Tile(-1,  0), Tile(0,  0), Tile(1,  0),
            Tile(-1, -1), Tile(0, -1), Tile(1, -1),
        },
        { OrangeTree(0, 0) }
    });

    // Small island
    // ~~~
    // ~#~
    // ~~~
    predefined_islands.push_back({
        fog_V2(0, 0), {
            Tile(0,  0),
        },
        {}
    });

    // Cresent island
    // ~~~~~~~~~
    // ~~####~~~
    // ~##~~##~~
    // ~~~~x~##~
    // ~~~~~~##~
    // ~##~~##~~
    // ~~####~~~
    // ~~~~~~~~~
    predefined_islands.push_back({
        fog_V2(0, 0), {
            Tile(-2,  2), Tile(-1,  2), Tile(0,  2), Tile(1,  2),
            Tile(-3,  1), Tile(-2,  1), Tile(1,  1), Tile(2,  1),
            Tile(2,  0), Tile(3,  0),
            Tile(2,  -1), Tile(3,  -1),
            Tile(-3,  -2), Tile(-2,  -2), Tile(1,  -2), Tile(2,  -2),
            Tile(-2,  -3), Tile(-1,  -3), Tile(0,  -3), Tile(1,  -3),
        },
        {}
    });

    // L island
    // ~~~~
    // ~#~~
    // ~#~~
    // ~##~
    // ~~~~
    predefined_islands.push_back({
        fog_V2(3, 3), {
            Tile(-1,  1),
            Tile(-1,  0),
            Tile(-1, -1), Tile(0, -1),
        },
        {}
    });

    int islands_left = 70;
    while (islands_left--) {
        Island to_place = predefined_islands[fog_random_int() % predefined_islands.size()];

        while (true) {
            Vec2 pos = fog_V2(fog_random_real(0.0, 1.0), fog_random_real(0.0, 1.0))
                * PirateMap().MAP_SIZE;
            pos.x = (int) pos.x;
            pos.y = (int) pos.y;
            bool can_place = true;
            for (Island& island : islands) {
                if (fog_distance_v2(island.position, pos) < 10) {
                    can_place = false;
                    break;
                }
            }

            if (can_place) {
                to_place.position = pos;
                for (OrangeTree& tree : to_place.trees) {
                    tree.oranges = fog_random_int() % 4;
                }
                islands.push_back(to_place);
                break;
            }
        }
    }
}

void island_draw(Island& island) {

    // Tiny island special case
    if (island.tiles.size() == 1) {
        fog_renderer_push_sprite_rect(
                0,                    // layer
                island.position,
                fog_V2(0.5, 0.5),     // scale (world)
                0,                    // rotation
                SMALL_ISLAND,
                fog_V2(0, 0),         // uv min
                fog_V2(512, 512),     // resolution
                fog_V4(1, 1, 1, 1));  // color
        return;
    }

    AssetID tile_ids[] = {
        TOP_LEFT,    TOP,    TOP_RIGHT,
        LEFT,        CENTER, RIGHT,
        BOTTOM_LEFT, BOTTOM, BOTTOM_RIGHT
    };

    for (Tile& tile : island.tiles) {
        Vec2 tile_pos = island.position + tile.rel_position * 0.5;
        int tile_index = 0;
        for (int j = 1; j >= -1; j--) {
            for (int i = -1; i <= 1; i++) {
                fog_renderer_push_sprite_rect(
                        0,                    // layer
                        tile_pos + fog_V2(i, j) * 0.5,
                        fog_V2(0.5, 0.5),     // scale (world)
                        0,                    // rotation
                        tile_ids[tile_index],
                        fog_V2(0, 0),         // uv min
                        fog_V2(512, 512),     // resolution
                        fog_V4(1, 1, 1, 1));  // color
                tile_index++;
            }
        }
    }

    for (OrangeTree& orange_tree : island.trees) {
        orange_tree.draw(island);
    }
}

void init_assets() {
    CENTER = fog_asset_fetch_id("GRASS");
    BOTTOM = fog_asset_fetch_id("GRASS_BOTTOM");
    BOTTOM_LEFT = fog_asset_fetch_id("GRASS_BOTTOM_LEFT");
    BOTTOM_RIGHT = fog_asset_fetch_id("GRASS_BOTTOM_RIGHT");
    LEFT = fog_asset_fetch_id("GRASS_LEFT");
    RIGHT = fog_asset_fetch_id("GRASS_RIGHT");
    TOP = fog_asset_fetch_id("GRASS_TOP");
    TOP_LEFT = fog_asset_fetch_id("GRASS_TOP_LEFT");
    TOP_RIGHT = fog_asset_fetch_id("GRASS_TOP_RIGHT");
    SMALL_ISLAND = fog_asset_fetch_id("GRASS_SMALL");
}

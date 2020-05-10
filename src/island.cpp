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

ShapeID TILE_SHAPE;

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
        { OrangeTree(0, 0) },
        ISLAND_STARTER
    });

    // Small island
    // ~~~
    // ~#~
    // ~~~
    predefined_islands.push_back({
        fog_V2(0, 0), {
            Tile(0,  0),
        },
        {},
        ISLAND_SMALL
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
            Tile(-2,  2), Tile(-1,  2), Tile( 0,  2), Tile(1,  2),
            Tile(-3,  1), Tile(-2,  1), Tile( 1,  1), Tile(2,  1),
            Tile( 2,  0), Tile( 3,  0),
            Tile( 2, -1), Tile( 3, -1),
            Tile(-3, -2), Tile(-2, -2), Tile( 1, -2), Tile(2, -2),
            Tile(-2, -3), Tile(-1, -3), Tile( 0, -3), Tile(1, -3),
        },
        {},
        ISLAND_L
    });

    // L island
    // ~~~~
    // ~#~~
    // ~#~~
    // ~##~
    // ~~~~
    predefined_islands.push_back({
        fog_V2(0, 0), {
            Tile(-1,  1),
            Tile(-1,  0),
            Tile(-1, -1), Tile(0, -1),
        },
        {},
        ISLAND_CRESENT
    });

    // Treasure Island
    // ~~~~~~~~~~
    // ~~~####~~~
    // ~~##~~##~~
    // ~##~~~~##~
    // ~#~~#~~~#~
    // ~#~~###~#~
    // ~##~~~###~
    // ~~##~~#~~~
    // ~~~####~~~
    // ~~~~~~~~~~
    predefined_islands.push_back({
        fog_V2(0, 0), {
                                      Tile(-1, 3), Tile(0, 3), Tile(1, 3), Tile(2, 3),
                         Tile(-2, 2), Tile(-1, 2),                         Tile(2, 2), Tile(3, 2),
            Tile(-3, 1), Tile(-2, 1),                                                  Tile(3, 1), Tile(4, 1),
            Tile(-3, 0),                           Tile(0, 0), Tile(1, 0),                                    Tile(4, 0),
            Tile(-3,-1),                           Tile(0,-1), Tile(1,-1), Tile(2,-1),             Tile(4,-1),
            Tile(-3,-2), Tile(-2,-2),                                      Tile(2,-2), Tile(3,-2), Tile(4,-2),
                         Tile(-2,-3), Tile(-1,-3),                         Tile(2,-3),
                                      Tile(-1,-4), Tile(0,-4), Tile(1,-4), Tile(2,-4),
        },
        { OrangeTree(0, 0) },
        ISLAND_TREASURE
    });


    // LiU island
    // ~~~~~~~~~
    // ~~######~
    // ~~####~~~
    // ~~#~~~~~~
    // ~~####~~~
    // ~~#~~~~~~
    // ~~####~~~
    // ~~~~~~~~~
    predefined_islands.push_back({
        fog_V2(0, 0), {
            Tile(-2, 2), Tile(-1, 2), Tile(0, 2), Tile(1, 2), Tile(2, 2), Tile(3, 2),
            Tile(-2, 1), Tile(-1, 1), Tile(0, 1), Tile(1, 1),
            Tile(-2, 0),
            Tile(-2,-1), Tile(-1,-1), Tile(0,-1), Tile(1,-1),
            Tile(-2,-2),
            Tile(-2,-3), Tile(-1,-3), Tile(0,-3), Tile(1,-3),
        },
        {},
        ISLAND_LIU
    });


    Island first_island = predefined_islands[0];
    first_island.position = fog_V2(64, 64) + fog_V2(2, 0);
    first_island.trees[0].body.position = first_island.position + first_island.trees[0].body.position;
    for (Tile& tile: first_island.tiles) {
        tile.body = fog_physics_create_body(TILE_SHAPE, 0.0, 1.0, 0.0);
        tile.body.scale = fog_V2(0.5, 0.5);
        tile.body.position = first_island.position + (tile.rel_position / 2);
    }
    islands.push_back(first_island);

    int islands_left = 69;
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
                    tree.body.position = pos + tree.body.position;
                }
                for (Tile& tile: to_place.tiles) {
                    tile.body = fog_physics_create_body(TILE_SHAPE, 0.0, 1.0, 0.0);
                    tile.body.scale = fog_V2(0.5, 0.5);
                    tile.body.position = to_place.position + (tile.rel_position / 2);
                }
                islands.push_back(to_place);
                break;
            }
        }
    }
}

void island_draw(Island& island) {
    //for (Tile& tile: island.tiles) {
    //    fog_physics_debug_draw_body(&tile.body);
    //}

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
        orange_tree.draw();
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

    Vec2 points[] = {
        fog_V2(0, 1),
        fog_V2(0, 0),
        fog_V2(1, 0),
        fog_V2(1, 1),
    };
    TILE_SHAPE = fog_physics_add_shape(4, points);
}

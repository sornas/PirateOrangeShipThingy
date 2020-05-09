#include "island.h"

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
            }});

    islands.push_back(predefined_islands[0]);
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
        Vec2 tile_pos = (island.position + tile.rel_position) * 0.5;
        int tile_index = 0;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
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
    SMALL_ISLAND = fog_renderer_push_sprite_rect("GRASS_SMALL");
}

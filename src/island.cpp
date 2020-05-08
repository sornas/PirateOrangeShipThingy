#include "island.h"

AssetID GRASS;
AssetID GRASS_BOTTOM;
AssetID GRASS_BOTTOM_LEFT;
AssetID GRASS_BOTTOM_RIGHT;
AssetID GRASS_LEFT;
AssetID GRASS_RIGHT;
AssetID GRASS_TOP;
AssetID GRASS_TOP_LEFT;
AssetID GRASS_TOP_RIGHT;

void island_init(std::vector<Island>& islands) {

    init_assets();
    std::vector<Island> predefined_islands;

    Vec2 x = fog_V2(1, 0);
    Vec2 y = fog_V2(0, 1);

    // Starter island
    // ~~~~~
    // ~###~
    // ~###~
    // ~###~
    // ~~~~~
    predefined_islands.push_back({fog_V2(0, 0), {
        Tile(-x + y, GRASS_TOP_LEFT),    Tile(y, GRASS_TOP),      Tile(x + y, GRASS_TOP_RIGHT),
        Tile(-x    , GRASS_LEFT), Tile(x*0, GRASS), Tile(x, GRASS_RIGHT),
        Tile(-x - y, GRASS_BOTTOM_LEFT), Tile(-y, GRASS_BOTTOM),  Tile(x - y, GRASS_BOTTOM_RIGHT),
    }});

    islands.push_back(predefined_islands[0]);
}

void island_draw(Island& island) {
    for (Tile& tile : island.tiles) {
        Vec2 tile_pos = (island.position + tile.rel_position) * 0.5;
        fog_renderer_push_sprite_rect(
                0,                    // layer
                tile_pos,
                fog_V2(0.5, 0.5),     // scale (world)
                0,                    // rotation
                tile.tile_id,
                fog_V2(0, 0),         // uv min
                fog_V2(512, 512),     // resolution
                fog_V4(1, 1, 1, 1));  // color
    }
}

void init_assets() {
    GRASS = fog_asset_fetch_id("GRASS");
    GRASS_BOTTOM = fog_asset_fetch_id("GRASS_BOTTOM");
    GRASS_BOTTOM_LEFT = fog_asset_fetch_id("GRASS_BOTTOM_LEFT");
    GRASS_BOTTOM_RIGHT = fog_asset_fetch_id("GRASS_BOTTOM_RIGHT");
    GRASS_LEFT = fog_asset_fetch_id("GRASS_LEFT");
    GRASS_RIGHT = fog_asset_fetch_id("GRASS_RIGHT");
    GRASS_TOP = fog_asset_fetch_id("GRASS_TOP");
    GRASS_TOP_LEFT = fog_asset_fetch_id("GRASS_TOP_LEFT");
    GRASS_TOP_RIGHT = fog_asset_fetch_id("GRASS_TOP_RIGHT");
}

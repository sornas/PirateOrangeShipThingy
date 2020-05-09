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
                Tile(-1,  1,  TOP_LEFT),    Tile(0,  1, TOP),     Tile(1,  1, TOP_RIGHT),
                Tile(-1,  0,  LEFT),        Tile(0,  0, CENTER),  Tile(1,  0, RIGHT),
                Tile(-1, -1,  BOTTOM_LEFT), Tile(0, -1, BOTTOM),  Tile(1, -1, BOTTOM_RIGHT),
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
    CENTER = fog_asset_fetch_id("GRASS");
    BOTTOM = fog_asset_fetch_id("GRASS_BOTTOM");
    BOTTOM_LEFT = fog_asset_fetch_id("GRASS_BOTTOM_LEFT");
    BOTTOM_RIGHT = fog_asset_fetch_id("GRASS_BOTTOM_RIGHT");
    LEFT = fog_asset_fetch_id("GRASS_LEFT");
    RIGHT = fog_asset_fetch_id("GRASS_RIGHT");
    TOP = fog_asset_fetch_id("GRASS_TOP");
    TOP_LEFT = fog_asset_fetch_id("GRASS_TOP_LEFT");
    TOP_RIGHT = fog_asset_fetch_id("GRASS_TOP_RIGHT");
}

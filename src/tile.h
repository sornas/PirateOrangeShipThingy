#include "fog.h"

#include <vector>

AssetID GRASS;
AssetID GRASS_BOTTOM;
AssetID GRASS_BOTTOM_LEFT;
AssetID GRASS_BOTTOM_RIGHT;
AssetID GRASS_LEFT;
AssetID GRASS_RIGHT;
AssetID GRASS_TOP;
AssetID GRASS_TOP_LEFT;
AssetID GRASS_TOP_RIGHT;

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

void draw_tile(AssetID tile, Vec2 position) {
    fog_renderer_push_sprite_rect(
            0,                    // layer
            position,
            fog_V2(0.5, 0.5),     // scale (world)
            0,                    // rotation
            tile,
            fog_V2(0, 0),         // uv min
            fog_V2(512, 512),     // resolution
            fog_V4(1, 1, 1, 1));  // color
}

#include "orange_tree.h"

AssetID ORANGE_TREE_BASE;
AssetID ORANGE_TREE_CROWN;
AssetID ORANGE_TREE_ORANGE;

void OrangeTree::draw(Island& island) {
    fog_renderer_push_sprite_rect(
        0,
        this->position + island.position,
        fog_V2(0.5, 0.5),
        0,
        ORANGE_TREE_BASE,
        fog_V2(0, 0),
        fog_V2(256, 512),
        fog_V4(1, 1, 1, 1)
    );

    fog_renderer_push_sprite_rect(
        0,
        this->position + island.position + fog_V2(0, 0.25),
        fog_V2(0.5, 0.5),
        0,
        ORANGE_TREE_CROWN,
        fog_V2(0, 0),
        fog_V2(512, 512),
        fog_V4(1, 1, 1, 1)
    );

    Vec2 orange_pos [] = { fog_V2(-0.1, 0.15), fog_V2(0, 0.25), fog_V2(0.1, 0.20) };
    for (int i = 0; i < this->oranges; i++) {
        fog_renderer_push_sprite_rect(
            0,
            this->position + island.position + orange_pos[i],
            fog_V2(0.07, 0.07),
            0,
            ORANGE_TREE_ORANGE,
            fog_V2(0, 0),
            fog_V2(512, 512),
            fog_V4(1, 1, 1, 1)
        );
    }
}

void init_orange_tree() {
    ORANGE_TREE_BASE = fog_asset_fetch_id("TREE_BASE");
    ORANGE_TREE_CROWN = fog_asset_fetch_id("TREE_CROWN");
    ORANGE_TREE_ORANGE = fog_asset_fetch_id("ORANGE");
}

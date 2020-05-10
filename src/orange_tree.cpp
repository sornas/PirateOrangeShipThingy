#include "orange_tree.h"

AssetID ORANGE_TREE_BASE;
AssetID ORANGE_TREE_CROWN;
AssetID ORANGE_TREE_ORANGE;

ShapeID orange_shape;

void set_up_orange_body() {
    Vec2 points[] = {
        fog_V2(0, 1),
        fog_V2(0, 0),
        fog_V2(1, 0),
        fog_V2(1, 1),
    };
    orange_shape = fog_physics_add_shape(4, points);
}

OrangeTree::OrangeTree(int x, int y) :
    body(fog_physics_create_body(orange_shape, 1.0, 0.1, 0.05)),
    oranges(fog_random_int() % 4)
{}

void OrangeTree::draw() {
    fog_renderer_push_sprite_rect(
        0,
        this->body.position,
        fog_V2(0.25, 0.5),
        0,
        ORANGE_TREE_BASE,
        fog_V2(0, 0),
        fog_V2(256, 512),
        fog_V4(1, 1, 1, 1)
    );

    fog_renderer_push_sprite_rect(
        0,
        this->body.position + fog_V2(0, 0.30),
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
            this->body.position + orange_pos[i],
            fog_V2(0.07, 0.07),
            0,
            ORANGE_TREE_ORANGE,
            fog_V2(0, 0),
            fog_V2(512, 512),
            fog_V4(1, 1, 1, 1)
        );
    }
}

std::vector<OrangeTree*> init_orange_trees(std::vector<Island>& islands) {
    ORANGE_TREE_BASE = fog_asset_fetch_id("TREE_BASE");
    ORANGE_TREE_CROWN = fog_asset_fetch_id("TREE_CROWN");
    ORANGE_TREE_ORANGE = fog_asset_fetch_id("ORANGE");

    std::vector<OrangeTree*> trees;
    for (Island& island : islands) {
        for (OrangeTree& tree : island.trees) {
            trees.push_back(&tree);
        }
    }

    return trees;
}

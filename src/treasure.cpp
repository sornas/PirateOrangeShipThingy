#include "treasure.h"

AssetID TREASURE_SPRITE;
ShapeID treasure_shape;

Treasure init_treasure(std::vector<Island>& islands) {
    {
        Vec2 points[] = {
            fog_V2(0, 1),
            fog_V2(0, 0),
            fog_V2(1, 0),
            fog_V2(1, 1),
        };
        treasure_shape = fog_physics_add_shape(4, points);
        TREASURE_SPRITE = fog_asset_fetch_id("CROSS");
    }

    Treasure treasure = { fog_physics_create_body(treasure_shape, 1, 0.1, 0.05) };

    Vec2 middle = fog_V2(1, 1) * 64;
    for (Island& island : islands) {
        if (fog_distance_v2(middle, island.position) < 10) {
            treasure.body.position = island.position + island.tiles[0].rel_position * 0.5;
        }
    }

    treasure.body.scale = fog_V2(0.25, 0.25);

    return treasure;
}

void Treasure::draw() {
    fog_renderer_push_sprite_rect(
        0,
        this->body.position,
        fog_V2(0.5, 0.5),
        0,
        TREASURE_SPRITE,
        fog_V2(0, 0),
        fog_V2(512, 512),
        fog_V4(1, 1, 1, 1)
    );
}

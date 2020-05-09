#include "fog.h"

#include "dude.h"

#define PI 3.1416

ShapeID dude_shape;

Dude init_dude(Vec2 position) {
    {
        Vec2 points[] = {
            fog_V2(0, 1),
            fog_V2(0, 0),
            fog_V2(1, 0),
            fog_V2(1, 1),
        };
        dude_shape = fog_physics_add_shape(4, points);
    }

    Body body = fog_physics_create_body(dude_shape, 1.0, 0.1, 0.05);
    body.rotation = -PI/2;
    body.scale = fog_V2(0.4, 0.4);

    return {
        body,
        1,  // speed
        1,  // visible
        0,  // walking
        fog_logic_now(),  // prev_switch
        0.1,  // frame_hold
        0,  // walking_id
        fog_asset_fetch_id("PIRATE_STILL"),
        fog_asset_fetch_id("PIRATE_WALKING_1"),
        fog_asset_fetch_id("PIRATE_WALKING_2"),
        0,  // cur_asset
    };
}

void Dude::update() {
    while (body.rotation < 0)    body.rotation += 2*PI;
    while (body.rotation > 2*PI) body.rotation -= 2*PI;

    fog_physics_integrate(&body, fog_logic_delta());
}

void Dude::draw() {
    if (!visible) return;

    fog_physics_debug_draw_body(&body);

    if (walking) {
        walking = 0;

        if (fog_logic_now() > prev_switch + frame_hold) {
            prev_switch = fog_logic_now();
            walking_asset_index = (walking_asset_index + 1) % 4;  // 4 states (see below)
            switch (walking_asset_index) {
            case 0:
                cur_asset = ASSET_STAND;
                break;
            case 1:
                cur_asset = ASSET_WALK_01;
                break;
            case 2:
                cur_asset = ASSET_STAND;
                break;
            case 3:
                cur_asset = ASSET_WALK_02;
                break;
            }
        }
    } else {
        cur_asset = ASSET_STAND;
    }

    fog_renderer_push_sprite_rect(
            0,
            body.position,
            fog_V2(0.4, 0.4),
            0,
            cur_asset,
            fog_V2(0, 0),
            fog_V2(512, 512),
            fog_V4(1, 1, 1, 1));
}

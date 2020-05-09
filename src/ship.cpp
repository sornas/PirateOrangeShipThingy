#include <math.h>

#include "fog.h"

#define PI 3.1415f

#include "ship.h"

ShapeID ship_shape;

Ship init_ship(Vec2 position) {
    {
        Vec2 points[] = {
            fog_V2(0, 1),
            fog_V2(0, 0),
            fog_V2(1, 0),
            fog_V2(1, 1),
        };
        ship_shape = fog_physics_add_shape(4, points);
    }

    Body body = fog_physics_create_body(ship_shape, 1.0, 0.1, 0.05);
    body.rotation = -PI/2;
    body.scale = fog_V2(0.4, 0.4);

    return {
        body,
        0.01f,  // speed
        0.005f,  // braking speed
        2.0f,  // rotation speed
        3.5f,  // velocity cap
        fog_asset_fetch_id("SHIP"),
    };
}

void Ship::update() {
    while (body.rotation < 0)    body.rotation += 2*PI;
    while (body.rotation > 2*PI) body.rotation -= 2*PI;

    //body.velocity += (body.velocity * -(fog_length_v2(this-body.>velocity)/max_velocity));
    //position += velocity;

    fog_physics_integrate(&body, fog_logic_delta());
}

void Ship::draw() {
    fog_physics_debug_draw_body(&body);

    s32 asset_scale_x = 0;
    f32 asset_rotation = 0;

    if (body.rotation < PI) {
        asset_scale_x = -1;
    } else {
        asset_scale_x = 1;
    }

    asset_rotation -= 0.05;
    asset_rotation += sin(fog_logic_now()*2) / 10;

    fog_renderer_push_sprite_rect(
            0,
            body.position,
            fog_V2(0.4 * asset_scale_x, 0.4),
            asset_rotation,
            ASSET_SHIP,
            fog_V2(0, 0),
            fog_V2(512, 512),
            fog_V4(1, 1, 1, 1));

    fog_renderer_push_line(1, body.position, body.position + (body.velocity), fog_V4(1, 0, 0, 1), 0.01);
}

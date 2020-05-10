#include <math.h>

#include "fog.h"
#include "main.h"

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
    body.position = fog_V2(64, 64);
    body.rotation = -PI/2;
    body.scale = fog_V2(0.4, 0.4);

    return {
        body,
        0.01f,  // speed
        0.015f,  // braking speed
        2.0f,  // rotation speed
        2.5f,  // velocity cap
        fog_asset_fetch_id("SHIP"),
    };
}

void Ship::update() {
    while (body.rotation < -PI * (3/2))    body.rotation += 2*PI;
    while (body.rotation > PI/2) body.rotation -= 2*PI;

    fog_physics_integrate(&body, fog_logic_delta());
    if (!(body.velocity == fog_V2(0, 0)))
        body.rotation = atan2(body.velocity.y, body.velocity.x) - PI/2;

}

void Ship::update_movement() {
    f32 delta = fog_logic_delta();

    if (fog_input_down(NAME(UP), P1)) {
        body.velocity += fog_rotate_v2(fog_V2(0, speed * (1 - fog_length_v2(body.velocity)/max_velocity)), body.rotation);
    }
    if (fog_input_down(NAME(DOWN), P1)) {
        if (fog_length_v2(body.velocity) > 0) {
            body.velocity -= fog_rotate_v2(fog_V2(0, braking_speed), body.rotation);
            if (fog_rotate_v2(body.velocity, -body.rotation).y < 0) {
                body.velocity = fog_V2(0, 0);
            }
        }
    }
    if (fog_input_down(NAME(LEFT), P1)) {
        body.rotation += rotation_speed * delta;
        body.velocity = fog_rotate_v2(body.velocity, rotation_speed * delta);
    }
    if (fog_input_down(NAME(RIGHT), P1)) {
        body.rotation -= rotation_speed * delta;
        body.velocity = fog_rotate_v2(body.velocity, -rotation_speed * delta);
    }
}

void Ship::draw() {
    fog_physics_debug_draw_body(&body);

    s32 asset_scale_x = 0;
    f32 asset_rotation = 0;

    if (body.rotation > 0 || body.rotation < -PI) {  // workaround for atan2
        asset_scale_x = -1;
    } else {
        asset_scale_x = 1;
    }

    asset_rotation -= 0.05;
    asset_rotation += sin(fog_logic_now()*2) / 10;

    fog_renderer_push_sprite_rect(
            0,
            body.position,
            fog_V2(body.scale.x * asset_scale_x, body.scale.y),
            asset_rotation,
            ASSET_SHIP,
            fog_V2(0, 0),
            fog_V2(512, 512),
            fog_V4(1, 1, 1, 1));

    fog_renderer_push_line(1, body.position, body.position + (body.velocity), fog_V4(1, 0, 0, 1), 0.01);
}

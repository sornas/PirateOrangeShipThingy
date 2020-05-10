#include "fog.h"

#include "dude.h"
#include "main.h"

#define PI 3.1416

ShapeID dude_shape;

AssetID STAR_SPR;

Dude init_dude() {
    STAR_SPR = fog_asset_fetch_id("STAR_SPR");

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
    body.scale = fog_V2(0.25, 0.25);

    ParticleSystem stars = fog_renderer_create_particle_system(3, 100, fog_V2(0, 0));
    stars.keep_alive = 0;
    stars.one_alpha = 1;
    stars.one_size = 0;
    stars.alive_time = { 1, 1 };
    stars.rotation = { 0, 0 };
    stars.angular_velocity = { 0, 2 };
    stars.velocity = { 0.5, 1 };
    stars.velocity_dir = { 0, PI };
    stars.acceleration = { 1, 2 };
    stars.acceleration_dir = { PI * 3/2, PI * 3/2 };
    stars.spawn_size = { 0.1, 0.15 };
    stars.spawn_alpha = { 1, 1 };
    stars.die_size = { 0, 0 };

    fog_renderer_particle_add_sprite(&stars, STAR_SPR);

    return {
        body,
        1,  // speed
        0,  // visible
        0,  // walking
        fog_logic_now(),  // prev_switch
        0.1,  // frame_hold
        0,  // walking_id
        fog_asset_fetch_id("PIRATE_STILL"),
        fog_asset_fetch_id("PIRATE_WALKING_1"),
        fog_asset_fetch_id("PIRATE_WALKING_2"),
        0,  // cur_asset
        stars,
    };
}

void Dude::spawn_stars() {
    stars.position = body.position;
    fog_renderer_particle_spawn(&stars, 20);
}

void Dude::update() {
    f32 delta = fog_logic_delta();

    while (body.rotation < 0)    body.rotation += 2*PI;
    while (body.rotation > 2*PI) body.rotation -= 2*PI;

    fog_physics_integrate(&body, delta);
}

Vec2 Dude::get_movement_delta() {
    Vec2 delta_pos = fog_V2(0, 0);
    if (fog_input_down(NAME(UP), P1)) {
        delta_pos.y += 1;
    }
    if (fog_input_down(NAME(DOWN), P1)) {
        delta_pos.y -= 1;
    }
    if (fog_input_down(NAME(LEFT), P1)) {
        delta_pos.x -= 1;
    }
    if (fog_input_down(NAME(RIGHT), P1)) {
        delta_pos.x += 1;
    }
    return delta_pos;
}

void Dude::draw() {
    if (!visible) return;

    //fog_physics_debug_draw_body(&body);

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
            fog_V2(0.2, 0.2),
            0,
            cur_asset,
            fog_V2(0, 0),
            fog_V2(512, 512),
            fog_V4(1, 1, 1, 1));
}

void Dude::reset_at(Vec2 position) {
    body.position = position;
    prev_switch = fog_logic_now();
    walking_asset_index = 0;
    cur_asset = 0;
}

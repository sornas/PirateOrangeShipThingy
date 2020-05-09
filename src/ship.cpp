#include <math.h>

#include "fog.h"

#define PI 3.1415f

#include "ship.h"

Ship init_ship(Vec2 position) {
    return {
        position,  // start position
        fog_V2(0, 0),  // start velocity
        -PI/2,  // start rotation
        0.01f,  // speed
        0.005f,  // braking speed
        2.0f,  // rotation speed
        1.0f,  // velocity cap
        fog_asset_fetch_id("SHIP_RIGHT"),
        fog_asset_fetch_id("SHIP_LEFT"),
    };
}

void Ship::update() {
    while (this->rotation < 0)    this->rotation += 2*PI;
    while (this->rotation > 2*PI) this->rotation -= 2*PI;

    this->velocity = this->velocity + (this->velocity * -(fog_length_v2(this->velocity)/this->max_velocity));
    this->position += this->velocity;
}

static inline
f32 lerp_f32(f32 a, f32 b, f32 l) {
    return a * (1.0f - l) + (b * l);
}

void Ship::draw() {
    s32 asset_scale_x = 0;
    f32 asset_rotation = 0;
    
    if (this->rotation > PI) {
        asset_scale_x = -1;
    } else {
        asset_scale_x = 1;
    }
    
    asset_rotation -= 0.05;
    asset_rotation += sin(fog_logic_now()*2) / 10;

    fog_renderer_push_sprite_rect(
            0,
            this->position,
            fog_V2(0.4 * asset_scale_x, 0.4),
            asset_rotation,
            SHIP_LEFT,
            fog_V2(0, 0),
            fog_V2(512, 512),
            fog_V4(1, 1, 1, 1));

    fog_renderer_push_line(1, this->position, this->position + (this->velocity * 30), fog_V4(1, 0, 0, 1), 0.01);
}

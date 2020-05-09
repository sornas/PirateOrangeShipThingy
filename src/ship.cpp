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

void Ship::draw() {
    AssetID asset;
    f32 asset_rotation;

    if (this->rotation < PI) {
        asset = SHIP_LEFT;
        asset_rotation = -PI/2;
    } else {
        asset = SHIP_RIGHT;
        asset_rotation = PI/2;
    }

    asset_rotation -= 0.05;
    asset_rotation += sin(fog_logic_now()*2) / 10;

    fog_renderer_push_sprite_rect(
            0,
            this->position,
            fog_V2(0.4, 0.4),
            this->rotation + asset_rotation,
            asset,
            fog_V2(0, 0),
            fog_V2(512, 512),
            fog_V4(1, 1, 1, 1));

    fog_renderer_push_line(1, this->position, this->position + (this->velocity * 30), fog_V4(1, 0, 0, 1), 0.01);
}

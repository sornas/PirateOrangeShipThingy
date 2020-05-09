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

    Body body = fog_physics_create_body(ship_shape, 1.0, 0.1, 0.4);
    body.rotation = -PI/2;
    body.scale = fog_V2(0.4, 0.4);

    return {
        body,
        0.01f,  // speed
        0.005f,  // braking speed
        2.0f,  // rotation speed
        1.0f,  // velocity cap
        fog_asset_fetch_id("SHIP"),
    };
}

void Ship::update() {
    while (this->body.rotation < 0)    this->body.rotation += 2*PI;
    while (this->body.rotation > 2*PI) this->body.rotation -= 2*PI;

    //this->body.velocity += (this->body.velocity * -(fog_length_v2(this-body.>velocity)/this->max_velocity));
    //this->position += this->velocity;

    fog_physics_integrate(&this->body, fog_logic_delta());
}

void Ship::draw() {
    fog_physics_debug_draw_body(&this->body);

    s32 asset_scale_x = 0;
    f32 asset_rotation = 0;

    if (this->body.rotation < PI) {
        asset_scale_x = -1;
    } else {
        asset_scale_x = 1;
    }

    asset_rotation -= 0.05;
    asset_rotation += sin(fog_logic_now()*2) / 10;

    fog_renderer_push_sprite_rect(
            0,
            this->body.position,
            fog_V2(0.4 * asset_scale_x, 0.4),
            asset_rotation,
            ASSET_SHIP,
            fog_V2(0, 0),
            fog_V2(512, 512),
            fog_V4(1, 1, 1, 1));

    fog_renderer_push_line(1, this->body.position, this->body.position + (this->body.velocity), fog_V4(1, 0, 0, 1), 0.01);
}

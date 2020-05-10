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

    ParticleSystem travel_particles = fog_renderer_create_particle_system(2, 500, fog_V2(0, 0));
    travel_particles.keep_alive = 0;
    travel_particles.one_color = 1;
    travel_particles.one_alpha = 0;
    travel_particles.one_size = 1;
    travel_particles.alive_time = { 1, 1 };
    travel_particles.rotation = { 0, 0 };
    travel_particles.angular_velocity = { 0, 0 };
    travel_particles.velocity = { 1, 1 };
    travel_particles.damping = { 0.5, 0.5 };
    travel_particles.acceleration = { 0, 0 };
    travel_particles.spawn_red = { 155/255.0, 155/255.0 };
    travel_particles.spawn_green = { 155/255.0, 155/255.0 };
    travel_particles.spawn_blue = { 244/255.0, 244/255.0 };
    travel_particles.spawn_size = { 0.02, 0.05 };
    travel_particles.spawn_alpha = { 0.2, 0.4 };
    travel_particles.die_alpha = { 0, 0 };

    return {
        body,
        0.01f,  // speed
        0.015f,  // braking speed
        2.0f,  // rotation speed
        2.5f,  // velocity cap
        travel_particles,
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
        travel_particles.position = body.position - fog_V2(0, body.scale.y/2.2);
        travel_particles.velocity_dir = { body.rotation + PI - PI/2 - PI/8, body.rotation + PI - PI/2 + PI/8};
        fog_renderer_particle_spawn(&travel_particles, 2);
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

    fog_renderer_particle_update(&travel_particles, delta);
}

void Ship::draw() {
    fog_renderer_particle_draw(&travel_particles);
    //fog_physics_debug_draw_body(&body);

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
            2,
            body.position,
            fog_V2(body.scale.x * asset_scale_x, body.scale.y),
            asset_rotation,
            ASSET_SHIP,
            fog_V2(0, 0),
            fog_V2(512, 512),
            fog_V4(1, 1, 1, 1));

    //fog_renderer_push_line(1, body.position, body.position + (body.velocity), fog_V4(1, 0, 0, 1), 0.01);
}

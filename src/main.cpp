#include "fog.h"

#include "main.h"

#include "island.h"

struct GamePlayer {
    Vec2 position;
    Vec2 velocity;
    f32 rotation;

    f32 speed;
    f32 braking_speed;
    f32 rotation_speed;

    f32 max_velocity;
};

struct GamePlayer player;

std::vector<Island> islands;

void init_game() {
    island_init(islands);
}

void update() {
    f32 delta = fog_logic_delta();

    if (fog_input_down(NAME(UP), P1))
        player.position += fog_V2(0, +player.speed) * delta;
    if (fog_input_down(NAME(DOWN), P1))
        player.position += fog_V2(0, -player.speed) * delta;
    if (fog_input_down(NAME(LEFT), P1))
        player.position += fog_V2(-player.speed, 0) * delta;
    if (fog_input_down(NAME(RIGHT), P1))
        player.position += fog_V2(+player.speed, 0) * delta;

    if (fog_input_down(NAME(UP), P1)) {
        player.velocity += fog_rotate_v2(fog_V2(0, player.speed * delta), player.rotation);
    }
    if (fog_input_down(NAME(DOWN), P1)) {
        player.velocity += fog_rotate_v2(fog_V2(0, -player.speed * delta / 2), player.rotation);
    }
    if (fog_input_down(NAME(LEFT), P1)) {
        player.rotation += player.rotation_speed * delta;
        player.velocity = fog_rotate_v2(player.velocity, player.rotation_speed * delta);
    }
    if (fog_input_down(NAME(RIGHT), P1)) {
        player.rotation -= player.rotation_speed * delta;
        player.velocity = fog_rotate_v2(player.velocity, -player.rotation_speed * delta);
    }

    player.velocity = player.velocity + (player.velocity * -(fog_length_v2(player.velocity)/player.max_velocity));
    player.position += player.velocity;

    fog_renderer_fetch_camera(0)->position = player.position;
}

void draw() {
    fog_renderer_push_point(1, player.position, fog_V4(1, 0, 0, 1), 0.1);   // ship
    fog_renderer_push_point(0, fog_V2(0.5, 0.5), fog_V4(0, 0, 0, 1), 0.3);  // island

    fog_renderer_push_line(1, player.position, player.position + (player.velocity * 30), fog_V4(1, 0, 0, 1), 0.01);

    for (Island& island : islands) {
        island_draw(island);
    }
}

int main(int argc, char **argv) {
    for (u32 i = 0; i < NUM_BINDINGS; i++) {
        bindings[i] = fog_input_request_name(1);
    }

    fog_init(argc, argv);
    fog_renderer_set_window_size(WIN_WIDTH, WIN_HEIGHT);
    fog_renderer_turn_on_camera(0);

    fog_input_add(fog_key_to_input_code(SDLK_w), NAME(UP), P1);
    fog_input_add(fog_key_to_input_code(SDLK_s), NAME(DOWN), P1);
    fog_input_add(fog_key_to_input_code(SDLK_a), NAME(LEFT), P1);
    fog_input_add(fog_key_to_input_code(SDLK_d), NAME(RIGHT), P1);

    player = {
        fog_V2(0, 0),
        fog_V2(0, 0),
        0.0f,
        0.01f,   // speed
        0.005f,  // braking speed
        2.0f,    // rotation

        1,       // "max velocity"
    };

    init_game();

    fog_run(update, draw);
    return 0;
}

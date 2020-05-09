#include "fog.h"

#include "main.h"

#include "island.h"
#include "ship.h"

#define PI 3.1415f

Ship ship;

std::vector<Island> islands;

void init_game() {
    island_init(islands);
}

void update() {
    f32 delta = fog_logic_delta();

    if (fog_input_down(NAME(UP), P1)) {
        ship.velocity += fog_rotate_v2(fog_V2(0, ship.speed * delta), ship.rotation);
    }
    if (fog_input_down(NAME(DOWN), P1)) {
        ship.velocity += fog_rotate_v2(fog_V2(0, -ship.speed * delta / 2), ship.rotation);
    }
    if (fog_input_down(NAME(LEFT), P1)) {
        ship.rotation += ship.rotation_speed * delta;
        ship.velocity = fog_rotate_v2(ship.velocity, ship.rotation_speed * delta);
    }
    if (fog_input_down(NAME(RIGHT), P1)) {
        ship.rotation -= ship.rotation_speed * delta;
        ship.velocity = fog_rotate_v2(ship.velocity, -ship.rotation_speed * delta);
    }
    ship.update();

    fog_renderer_fetch_camera(0)->position = ship.position;
}

void draw() {
    for (Island& island : islands) {
        island_draw(island);
    }

    ship.draw();
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

    init_game();
    ship = init_ship(fog_V2(0, 0));

    fog_run(update, draw);
    return 0;
}

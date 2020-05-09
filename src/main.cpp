#include <math.h>

#define FOG_IMPL
#include "fog.h"

#include "main.h"

#include "island.h"
#include "ship.h"
#include "pirate_map.h"

#define PI 3.1415f

Ship ship;

std::vector<Island> islands;

PirateMap pirate_map;


AssetID WATER;

AssetID SONG_01;
//AssetID SONG_02;
AudioID music_background;

void init_game() {
    //island_init(islands);
    ship = init_ship(fog_V2(0, 0));
    WATER = fog_asset_fetch_id("SEA");

    SONG_01 = fog_asset_fetch_id("SONG_01_16K");
    //SONG_02 = fog_asset_fetch_id("SONG-02-16K");

    pirate_map = PirateMap();
}

void update() {
    f32 delta = fog_logic_delta();

    if (fog_input_down(NAME(UP), P1)) {
        ship.body.velocity += fog_rotate_v2(fog_V2(0, ship.speed), ship.body.rotation);
    }
    if (fog_input_down(NAME(DOWN), P1)) {
        ship.body.velocity += fog_rotate_v2(fog_V2(0, -ship.braking_speed), ship.body.rotation);
    }
    if (fog_input_down(NAME(LEFT), P1)) {
        ship.body.rotation += ship.rotation_speed * delta;
        ship.body.velocity = fog_rotate_v2(ship.body.velocity, ship.rotation_speed * delta);
    }
    if (fog_input_down(NAME(RIGHT), P1)) {
        ship.body.rotation -= ship.rotation_speed * delta;
        ship.body.velocity = fog_rotate_v2(ship.body.velocity, -ship.rotation_speed * delta);
    }
    ship.update();

    fog_util_show_f32("velocity", fog_length_v2(ship.body.velocity));

    fog_renderer_fetch_camera(0)->position = ship.body.position;
    fog_renderer_fetch_camera(0)->zoom = 0.7 - 0.2 * (fog_length_v2(ship.body.velocity) / 3.0);
}

void draw() {
    Vec2 offset = fog_V2(-sin(2*fog_logic_now())/40, 0);

    // Draw surrounding water
    for (int i = -8; i <= 8; i++) {
        for (int j = -8; j <= 8; j++) {
            Vec2 water_pos = fog_V2((int) ship.body.position.x, (int) ship.body.position.y) + fog_V2(i, j) * 0.5;
            fog_renderer_push_sprite_rect(
                    0,
                    water_pos + offset,
                    fog_V2(0.5, 0.5),
                    0,
                    WATER,
                    fog_V2(0, 0),
                    fog_V2(512, 512),
                    fog_V4(1, 1, 1, 1));
        }
    }

    for (Island& island : islands) {
        island_draw(island);
    }

    ship.draw();

    draw_pirate_map(pirate_map);
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

    music_background = fog_mixer_play_sound(0, SONG_01, 1.0, AUDIO_DEFAULT_GAIN, AUDIO_DEFAULT_VARIANCE, AUDIO_DEFAULT_VARIANCE, 1);

    fog_run(update, draw);
    return 0;
}

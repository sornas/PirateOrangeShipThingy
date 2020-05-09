#include <math.h>
#include <time.h>

#define FOG_IMPL
#include "fog.h"

#include "main.h"

#include "island.h"
#include "ship.h"
#include "dude.h"
#include "pirate_map.h"

#define PI 3.1415f
#include <iostream>

Ship ship;
Dude dude;

b8 controlling_dude = 0;

std::vector<Island> islands;

PirateMap pirate_map;

AssetID WATER;

AudioID music_id;
AssetID SONG_01;
AssetID SONG_02;
AssetID SONG_FOG;

int VISION = 4;

void init_game() {
    fog_random_seed(time(NULL));
    island_init(islands);
    ship = init_ship(fog_V2(0, 0));
    dude = init_dude();

    WATER = fog_asset_fetch_id("SEA");

    SONG_01 = fog_asset_fetch_id("SONG_01_16K");
    SONG_02 = fog_asset_fetch_id("SONG-02-16K");
    SONG_FOG = fog_asset_fetch_id("FOGGIESTOFSONGS_8K");

    init_orange_tree();
    pirate_map = PirateMap(islands);
}

void update() {
    f32 delta = fog_logic_delta();

    //TODO(gu) ideally input should be checked by the respective updates
    if (!controlling_dude) {
        if (fog_input_down(NAME(UP), P1)) {
            ship.body.velocity += fog_rotate_v2(fog_V2(0, ship.speed * (1 - fog_length_v2(ship.body.velocity)/ship.max_velocity)), ship.body.rotation);
        }
        if (fog_input_down(NAME(DOWN), P1)) {
            if (fog_length_v2(ship.body.velocity) > 0) {
                ship.body.velocity -= fog_rotate_v2(fog_V2(0, ship.braking_speed), ship.body.rotation);
                if (fog_rotate_v2(ship.body.velocity, -ship.body.rotation).y < 0) {
                    ship.body.velocity = fog_V2(0, 0);
                }
            }
        }
        if (fog_input_down(NAME(LEFT), P1)) {
            ship.body.rotation += ship.rotation_speed * delta;
            ship.body.velocity = fog_rotate_v2(ship.body.velocity, ship.rotation_speed * delta);
        }
        if (fog_input_down(NAME(RIGHT), P1)) {
            ship.body.rotation -= ship.rotation_speed * delta;
            ship.body.velocity = fog_rotate_v2(ship.body.velocity, -ship.rotation_speed * delta);
        }

        fog_util_show_f32("ship vel size", fog_length_v2(ship.body.velocity));
        if (fog_input_pressed(NAME(TOGGLE_SHIP), P1)) {
            if (fog_length_v2(ship.body.velocity) < 0.15) {
                ship.body.velocity = fog_V2(0, 0);
                controlling_dude = 1;
                dude.reset_at(ship.body.position);
                dude.visible = 1;
                fog_mixer_stop_sound(music_id);
                music_id = fog_mixer_play_sound(0, SONG_01, 1.0,
                        AUDIO_DEFAULT_GAIN, AUDIO_DEFAULT_VARIANCE, AUDIO_DEFAULT_VARIANCE, 1);
            }
        }

        fog_renderer_fetch_camera(0)->position = ship.body.position;
        fog_renderer_fetch_camera(0)->zoom = 0.7 - 0.2 * (fog_length_v2(ship.body.velocity) / 3.0);
    } else {  // controlling dude
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
        if (!(delta_pos == fog_V2(0, 0))) {
            dude.walking = 1;
            if (fog_length_v2(delta_pos) > 1) {
                dude.body.position += fog_normalize_v2(delta_pos) * dude.speed * delta;
            } else {
                dude.body.position += fog_normalize_v2(delta_pos) * dude.speed * delta;
            }
        }

        if (fog_input_pressed(NAME(TOGGLE_SHIP), P1)) {
            if (fog_physics_check_overlap(&dude.body, &ship.body).is_valid) {
                controlling_dude = 0;
                dude.visible = 0;
                fog_mixer_stop_sound(music_id);
                music_id = fog_mixer_play_sound(0, SONG_FOG, 1.0,
                        AUDIO_DEFAULT_GAIN, AUDIO_DEFAULT_VARIANCE, AUDIO_DEFAULT_VARIANCE, 1);
            }
        }

        dude.update();
        fog_renderer_fetch_camera(0)->position = dude.body.position;
        fog_renderer_fetch_camera(0)->zoom = 1.5;
    }

    // Discover surrounding area
    for (int i = -VISION; i <= VISION; i++) {
        for (int j = -VISION; j <= VISION; j++) {
            Vec2 discovered_pos = ship.body.position + fog_V2(i, j);
            pirate_map.discover(discovered_pos);
        }
    }

    ship.update();  //(gu): ship should always update, not only when controlled
}

void draw() {
    Vec2 offset = fog_V2(-sin(2*fog_logic_now())/40, 0);

    // Draw surrounding water
    for (int i = -2*VISION; i <= 2*VISION; i++) {
        for (int j = -2*VISION; j <= 2*VISION; j++) {
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
        if (fog_distance_v2(island.position, ship.body.position) < 15) {
            island_draw(island);
        }
    }

    ship.draw();
    dude.draw();

    draw_pirate_map(pirate_map, ship.body.position);

    float text_alpha;
    if (fog_logic_now() < 10) {
        text_alpha = 1;
    } else {
        text_alpha = (20 - fog_logic_now()) / 10;
        text_alpha = text_alpha < 0 ? 0 : text_alpha;
    }
        fog_renderer_draw_text("There is a treasure out there...", -0.9, -0.5, 0.6, fog_asset_fetch_id("MONACO_FONT"), 0, fog_V4(1, 1, 1, text_alpha), 0.1, false);
        fog_renderer_draw_text("wasd to move - e to interact / leave ship", -0.9, -0.6, 0.6, fog_asset_fetch_id("MONACO_FONT"), 0, fog_V4(1, 1, 1, text_alpha), 0.1, false);
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
    fog_input_add(fog_key_to_input_code(SDLK_SPACE), NAME(TOGGLE_SHIP), P1);

    init_game();

    music_id = fog_mixer_play_sound(0, SONG_FOG, 1.0, AUDIO_DEFAULT_GAIN, AUDIO_DEFAULT_VARIANCE, AUDIO_DEFAULT_VARIANCE, 1);

    fog_run(update, draw);
    return 0;
}

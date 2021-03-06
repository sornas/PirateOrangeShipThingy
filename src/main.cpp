#include <math.h>
#include <time.h>

#define FOG_IMPL
#include "fog.h"

#include "main.h"

#include "island.h"
#include "ship.h"
#include "dude.h"
#include "pirate_map.h"
#include "treasure.h"

#define PI 3.1415f

Ship ship;
Dude dude;

b8 controlling_dude = 0;

std::vector<Island> islands;
std::vector<OrangeTree*> orange_trees;

PirateMap pirate_map;

Treasure treasure;

AssetID WATER;

AudioID music_id;
AssetID SONG_01;
AssetID SONG_02;
AssetID SONG_FOG;

int VISION = 10;

bool game_over;
bool found_treasure;

float scurvy;
float scurvy_decay = 1.0 / 180;

void init_game() {
    fog_random_seed(time(NULL));

    set_up_orange_body();

    island_init(islands);
    treasure = init_treasure(islands);
    ship = init_ship(fog_V2(0, 0));
    dude = init_dude();

    WATER = fog_asset_fetch_id("SEA");

    SONG_01 = fog_asset_fetch_id("SONG_01_16K");
    //SONG_02 = fog_asset_fetch_id("SONG-02-16K");
    SONG_FOG = fog_asset_fetch_id("FOGGIESTOFSONGS_8K");

    orange_trees = init_orange_trees(islands);
    pirate_map = PirateMap(islands);

    game_over = false;
    found_treasure = false;
    scurvy = 1;
}

void update() {
    f32 delta = fog_logic_delta();

    if (game_over) {
        return;
    }

    //TODO(gu) ideally input should be checked by the respective updates
    if (!controlling_dude) {
        ship.update_movement();

        if (fog_input_pressed(NAME(INTERACT), P1)) {
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
        for (Island& island: islands) {
            if (fog_distance_v2(island.position, ship.body.position) < 15) {
                for (Tile& tile: island.tiles) {
                    Overlap overlap = fog_physics_check_overlap(&tile.body, &ship.body);
                    if (overlap.is_valid) {
                        ship.body.velocity *= 0.90;
                        break;
                        //fog_physics_solve(overlap);
                    }
                }
            }
        }

        fog_renderer_fetch_camera(0)->position = ship.body.position;
        fog_renderer_fetch_camera(0)->zoom = 0.7 - 0.2 * (fog_length_v2(ship.body.velocity) / 3.0);
    } else {  // controlling dude
        Vec2 delta_pos = dude.get_movement_delta();
        if (!(delta_pos == fog_V2(0, 0))) {
            Vec2 prev_pos = dude.body.position;
            dude.walking = 1;

            if (fog_length_v2(delta_pos) > 1) {
                dude.body.position += fog_normalize_v2(delta_pos) * dude.speed * delta;
            } else {
                dude.body.position += fog_normalize_v2(delta_pos) * dude.speed * delta;
            }

            b8 can_walk_to = 0;
            for (Island& island: islands) {
                if (fog_distance_v2(island.position, dude.body.position) < 10) {
                    for (Tile& tile: island.tiles) {
                        if (fog_physics_check_overlap(&tile.body, &dude.body).is_valid) {
                            can_walk_to = 1;
                            break;
                        }
                    }
                }
            }
            can_walk_to |= fog_physics_check_overlap(&ship.body, &dude.body).is_valid;

            if (!can_walk_to) {
                dude.walking = 0;
                dude.body.position = prev_pos;
            }
        }

        if (fog_input_pressed(NAME(INTERACT), P1)) {
            if (fog_physics_check_overlap(&dude.body, &ship.body).is_valid) {
                controlling_dude = 0;
                dude.visible = 0;
                fog_mixer_stop_sound(music_id);
                music_id = fog_mixer_play_sound(0, SONG_FOG, 1.0,
                        AUDIO_DEFAULT_GAIN, AUDIO_DEFAULT_VARIANCE, AUDIO_DEFAULT_VARIANCE, 1);
            } else if (fog_physics_check_overlap(&dude.body, &treasure.body).is_valid) {
                game_over = true;
                found_treasure = true;
            } else {
                for (OrangeTree* tree : orange_trees) {
                    if (fog_physics_check_overlap(&dude.body, &tree->body).is_valid && tree->oranges) {
                        dude.spawn_stars();
                        tree->oranges--;
                        scurvy = 1;
                        break;
                    }
                }
            }
        }

        dude.update();
        fog_renderer_fetch_camera(0)->position = dude.body.position;
        fog_renderer_fetch_camera(0)->zoom = 1.5;
    }

    fog_renderer_particle_update(&dude.stars, delta);

    // Discover surrounding area
    for (int i = -VISION; i <= VISION; i++) {
        for (int j = -VISION; j <= VISION; j++) {
            Vec2 discovered_pos = ship.body.position + fog_V2(i, j);
            pirate_map.discover(discovered_pos);
        }
    }

    ship.update();  //(gu): ship should always update, not only when controlled

    scurvy -= scurvy_decay * (0.5 + scurvy * 0.5) * delta;
    if (scurvy < 0) {
        scurvy = 0;
        game_over = true;
    }
}

void draw() {
    fog_renderer_particle_draw(&dude.stars);

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

    //fog_renderer_push_line(15, fog_V2(0, 0), fog_V2(0, 128), fog_V4(1, 0, 1, 1), 0.05);
    //fog_renderer_push_line(15, fog_V2(0, 0), fog_V2(128, 0), fog_V4(1, 0, 1, 1), 0.05);
    //fog_renderer_push_line(15, fog_V2(128, 128), fog_V2(128, 0), fog_V4(1, 0, 1, 1), 0.05);
    //fog_renderer_push_line(15, fog_V2(128, 128), fog_V2(128, 0), fog_V4(1, 0, 1, 1), 0.05);

    treasure.draw();
    ship.draw();
    dude.draw();

    // Line to treasure for debugging
    //fog_renderer_push_line(0, ship.body.position, treasure.body.position, fog_V4(1, 0, 0, 1), 0.01);

    draw_pirate_map(pirate_map, ship.body.position);

    // Draw scurvy meter
    Camera *camera = fog_renderer_fetch_camera(0);
    Vec2 scurvy_position = camera->position;
    scurvy_position = scurvy_position - fog_V2(0.5, camera->aspect_ratio) * 0.9 / camera->zoom;

    Vec2 meter_width = fog_V2(1.0, 0.0) / camera->zoom;
    Vec2 meter_height = fog_V2(0.0, 0.07) / camera->zoom;
    Vec2 meter_top_left = scurvy_position + (meter_height - meter_width) * 0.5;
    float border_width = 0.005 / camera->zoom;
    //fog_renderer_push_rectangle(
    //    0,
    //    scurvy_position,
    //    fog_V2(1, 0.07) / camera->zoom,
    //    fog_V4(0.8, 0.3, 0, 1)
    //);

    fog_renderer_push_rectangle(
        0,
        scurvy_position - fog_V2(1 - scurvy, 0) * 0.5 / camera->zoom,
        fog_V2(scurvy, 0.07) / camera->zoom,
        fog_V4(178, 120, 53, 255) / 255
    );

    fog_renderer_push_line(0, meter_top_left, meter_top_left + meter_width, fog_V4(1, 1, 1, 1), border_width);
    fog_renderer_push_line(0, meter_top_left - meter_height, meter_top_left - meter_height + meter_width, fog_V4(1, 1, 1, 1), border_width);
    fog_renderer_push_line(0, meter_top_left + meter_width, meter_top_left + meter_width - meter_height, fog_V4(1, 1, 1, 1), border_width);
    fog_renderer_push_line(0, meter_top_left, meter_top_left - meter_height, fog_V4(1, 1, 1, 1), border_width);

    fog_renderer_draw_text("VITAMIN C", -0.92, -0.8725, 0.5,
            fog_asset_fetch_id("MONACO_FONT"), 0, fog_V4(1, 1, 1, 1), 0.1,
            false);

    float text_alpha;
    if (fog_logic_now() < 10) {
        text_alpha = 1;
    } else {
        text_alpha = (20 - fog_logic_now()) / 10;
        text_alpha = text_alpha < 0 ? 0 : text_alpha;
    }

    if (text_alpha) {
        fog_renderer_draw_text("There is a treasure out there...",          -0.9, -0.5, 0.6, fog_asset_fetch_id("MONACO_FONT"), 0, fog_V4(1, 1, 1, text_alpha), 0.1, false);
        fog_renderer_draw_text("WASD to move - E to interact / leave ship", -0.9, -0.6, 0.6, fog_asset_fetch_id("MONACO_FONT"), 0, fog_V4(1, 1, 1, text_alpha), 0.1, false);
        fog_renderer_draw_text("F10 to exit",                               -0.9, -0.7, 0.6, fog_asset_fetch_id("MONACO_FONT"), 0, fog_V4(1, 1, 1, text_alpha), 0.1, false);
    }

    if (game_over) {
        const char* text1 = found_treasure ? "You found the fabled treasure" :
            "You have fallen due to scurvy";
        const char* text2 = found_treasure ? "But at what cost?" :
            "There is still a treasure out there...";
        fog_renderer_draw_text(text1, -0.9, -0.5, 0.6, fog_asset_fetch_id("MONACO_FONT"), 0, fog_V4(1, 1, 1, 1), 0.1, false);
        fog_renderer_draw_text(text2, -0.9, -0.6, 0.6, fog_asset_fetch_id("MONACO_FONT"), 0, fog_V4(1, 1, 1, 1), 0.1, false);
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
    fog_input_add(fog_key_to_input_code(SDLK_e), NAME(INTERACT), P1);

    init_game();

    music_id = fog_mixer_play_sound(0, SONG_FOG, 1.0, AUDIO_DEFAULT_GAIN, AUDIO_DEFAULT_VARIANCE, AUDIO_DEFAULT_VARIANCE, 1);

    fog_run(update, draw);
    return 0;
}

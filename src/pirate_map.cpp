#include "pirate_map.h"
#include <cstdlib>
#include <iostream>

void draw_pirate_map(PirateMap& pirate_map) {
    Vec2 pos = fog_input_screen_to_world(fog_V2(0, 0), 0);

    Image* img = fog_asset_fetch_image(pirate_map.image_id);
    img->data = (u8*) malloc(img->width * img->height * img->components);
    for (int i = 0; i < pirate_map.MAP_SIZE; i++) {
        for (int j = 0; j < pirate_map.MAP_SIZE; j++) {
            img->data[img->components * (i * img->width + j) + 0] = 0;
            img->data[img->components * (i * img->width + j) + 1] = 0;
            img->data[img->components * (i * img->width + j) + 2] = 0;
        }
    }

    pirate_map.image_id = fog_upload_texture(img, pirate_map.image_id);
    free(img->data);

    std::cout << pirate_map.image_id << std::endl;
    fog_renderer_push_sprite_rect(
            0,
            pos,
            fog_V2(1, 1),
            0,
            pirate_map.image_id,
            fog_V2(0, 0),
            fog_V2(128, 128),
            fog_V4(1, 1, 1, 1));
    //for (int i = 0; i < pirate_map.MAP_SIZE; i++) {
    //    for (int j = 0; j < pirate_map.MAP_SIZE; j++) {
    //        fog_renderer_push_rectangle(
    //            0,
    //            fog_input_screen_to_world(fog_V2(j, i), 0),
    //            fog_V2(1, 1) * pirate_map.TILE_SIZE,
    //            fog_V4(1, 1, 1, 1)
    //        );
    //    }
    //}
}

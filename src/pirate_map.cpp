#include "pirate_map.h"
#include <cstdlib>
#include <iostream>

void draw_pirate_map(PirateMap& pirate_map) {
    Vec2 pos = fog_V2(0, 0);

    Image* img = fog_asset_fetch_image(pirate_map.image_id);
    img->data = (u8*) malloc(pirate_map.MAP_SIZE * pirate_map.MAP_SIZE * img->components);
    for (int i = 0; i < pirate_map.MAP_SIZE; i++) {
        for (int j = 0; j < pirate_map.MAP_SIZE; j++) {
            for (int k = 0; k < img->components; k++) {
                img->data[img->components * (i * img->width + j) + k] = i % 256;
            }
        }
    }

    fog_upload_texture(img, img->id);
    free(img->data);

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

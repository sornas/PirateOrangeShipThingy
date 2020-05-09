#include "pirate_map.h"
#include <cstdlib>
#include <iostream>

void draw_pirate_map(PirateMap& pirate_map) {
    Camera *camera = fog_renderer_fetch_camera(0);
    Vec2 pos = camera->position;

    Image* img = fog_asset_fetch_image(pirate_map.image_id);
    img->data = (u8*) malloc(pirate_map.MAP_SIZE * pirate_map.MAP_SIZE * img->components);
    for (int x = 0; x < pirate_map.MAP_SIZE; x++) {
        for (int y = 0; y < pirate_map.MAP_SIZE; y++) {
            Vec3 color = color_from_map_tile(pirate_map.tiles[y][x]) * pirate_map.discovered[y][x];
            img->data[img->components * (x * img->width + y) + 0] = (u8)color.x;
            img->data[img->components * (x * img->width + y) + 1] = (u8)color.y;
            img->data[img->components * (x * img->width + y) + 2] = (u8)color.z;
        }
    }

    fog_upload_texture(img, img->id);
    free(img->data);

    fog_renderer_push_sprite_rect(
            0,
            pos,
            fog_V2(1, 1) / camera->zoom,
            0,
            pirate_map.image_id,
            fog_V2(0, 0),
            fog_V2(128, 128),
            fog_V4(1, 1, 1, 1));
}

Vec3 color_from_map_tile(MapTile map_tile) {
    switch (map_tile) {
        case WATER_TILE:
            return fog_V3(0, 128, 255);
        case GRASS_TILE:
            return fog_V3(0, 255, 0);
        default:
            return fog_V3(0, 0, 0);
    }
}

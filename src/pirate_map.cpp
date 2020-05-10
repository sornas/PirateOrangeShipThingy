#include "pirate_map.h"

void draw_pirate_map(PirateMap& pirate_map, Vec2 ship_position) {
    Camera *camera = fog_renderer_fetch_camera(0);
    Vec2 camera_pos = camera->position;

    Image *img = fog_asset_fetch_image(pirate_map.image_id);
    img->data = pirate_map.map_data;

    for (int x = 0; x < pirate_map.MAP_SIZE; x++) {
        for (int y = 0; y < pirate_map.MAP_SIZE; y++) {
            Vec3 color = color_from_map_tile(pirate_map.tiles[y][x]) * pirate_map.discovered[y][x];
            img->data[img->components * (y * img->width + x) + 0] = (u8) color.x;
            img->data[img->components * (y * img->width + x) + 1] = (u8) color.y;
            img->data[img->components * (y * img->width + x) + 2] = (u8) color.z;
        }
    }
    fog_upload_texture(img, img->id);

    Vec2 map_position =
        camera_pos
        + (fog_V2(1, camera->aspect_ratio)
           / camera->zoom)
        * (1 - pirate_map.MAP_SCALE/2);

    fog_renderer_push_sprite_rect(
            0,
            map_position,
            fog_V2(1, 1) * pirate_map.MAP_SCALE / camera->zoom,
            0,
            pirate_map.image_id,
            fog_V2(0, 0),
            fog_V2(128, 128),
            fog_V4(1, 1, 1, 1));

    fog_renderer_push_point(
            0,
            map_position
            + ((ship_position - fog_V2(64, 64))
               * pirate_map.MAP_SCALE
               / pirate_map.MAP_SIZE
               / camera->zoom),
            fog_V4(150, 59, 51, 255) / 255,
            0.015 / camera->zoom
    );

}

Vec3 color_from_map_tile(MapTile map_tile) {
    switch (map_tile) {
        case WATER_TILE:
            return fog_V3(56, 56, 113);
        case GRASS_TILE:
            return fog_V3(66, 120, 51);
        default:
            return fog_V3(0, 0, 0);
    }
}

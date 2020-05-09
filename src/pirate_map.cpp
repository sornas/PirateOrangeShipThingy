#include "pirate_map.h"
#include <cstdlib>
#include <iostream>

void draw_pirate_map(PirateMap& pirate_map, Ship& ship) {
    Camera *camera = fog_renderer_fetch_camera(0);
    Vec2 pos = camera->position;

    float point_size = 0.004;
    Vec2 map_offset = pos + fog_V2(1, camera->aspect_ratio) * (1 - point_size *
            pirate_map.MAP_SIZE) / camera->zoom;

    for (int x = 0; x < pirate_map.MAP_SIZE; x++) {
        for (int y = 0; y < pirate_map.MAP_SIZE; y++) {
            Vec2 rel_pos = fog_V2(x, y);
            Vec3 color = color_from_map_tile(pirate_map.tiles[y][x]) * pirate_map.discovered[y][x];
            fog_renderer_push_point(
                0,
                map_offset + rel_pos * point_size / camera->zoom,
                fog_V4(color.x, color.y, color.z, 1),
                point_size / camera->zoom
            );
        }
    }

    fog_renderer_push_point(
            0,
            map_offset + ship.body.position * point_size / camera->zoom,
            fog_V4(1, 0, 0, 1),
            point_size / camera->zoom
    );
}

Vec3 color_from_map_tile(MapTile map_tile) {
    switch (map_tile) {
        case WATER_TILE:
            return fog_V3(0, 128, 255) / 255.0;
        case GRASS_TILE:
            return fog_V3(0, 196, 0) / 255.0;
        default:
            return fog_V3(0, 0, 0);
    }
}

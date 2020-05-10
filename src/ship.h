#pragma once
struct Ship {
    Body body;

    f32 speed;
    f32 braking_speed;
    f32 rotation_speed;

    f32 max_velocity;

    AssetID ASSET_SHIP;

    void update();
    void update_movement();
    void draw();
};

Ship init_ship(Vec2 position);

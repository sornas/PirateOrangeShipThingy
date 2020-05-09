struct Ship {
    Vec2 position;
    Vec2 velocity;
    f32 rotation;

    f32 speed;
    f32 braking_speed;
    f32 rotation_speed;

    f32 max_velocity;

    AssetID SHIP_RIGHT;
    AssetID SHIP_LEFT;

    void update();
    void draw();
};

Ship init_ship(Vec2 position);

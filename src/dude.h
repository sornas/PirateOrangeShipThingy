struct Dude {
    Body body;

    f32 speed;
    b8 visible;
    b8 walking;

    f32 prev_switch;
    f32 frame_hold;
    u32 walking_asset_index;
    AssetID ASSET_STAND;
    AssetID ASSET_WALK_01;
    AssetID ASSET_WALK_02;
    AssetID cur_asset;

    void update();
    Vec2 get_movement_delta();
    void draw();
    void reset_at(Vec2 position);
};

Dude init_dude();

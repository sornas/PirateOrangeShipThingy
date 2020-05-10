#pragma once

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    INTERACT,
    NUM_BINDINGS
} Binding;
inline Name bindings[NUM_BINDINGS];

#define NAME(binding) bindings[binding]

#define WIN_WIDTH 800
#define WIN_HEIGHT 800

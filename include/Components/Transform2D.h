#pragma once
#include <raylib.h>

struct Transform2D {
    Vector2 position{400, 225};
    Vector2 velocity{0, 0};
    float   speed = 140.0f;   // px/s
};

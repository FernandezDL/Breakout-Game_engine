#pragma once
#include "raylib.h"

struct Camera2DComponent {
    Camera2D cam{};

    float lerp = 10.0f;    
    Vector2 offset{ 0, 0 }; 
    float zoom = 1.0f;

    Rectangle worldBounds{ 0, 0, 4096, 4096 };    // Límites del mundo

    float shakeTime = 0.0f;  
    float shakeStrength = 0.0f;
};
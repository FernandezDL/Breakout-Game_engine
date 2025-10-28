#pragma once
#include <raylib.h>
#include <string>

struct Sprite {
    Texture2D* texture = nullptr;  // puntero a textura cargada
    Rectangle  src{};              // recorte actual del spritesheet
    Vector2    origin{0,0};
    float      scale = 1.0f;
    std::string currentAnim;     
};

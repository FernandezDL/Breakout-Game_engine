#pragma once
#include <entt/entt.hpp>
#include <raylib.h>

class Scene {
public:
    entt::registry reg;

    Texture2D bg{0};
    Texture2D hero{0};
    // Texture2D slime{0};

    void setup();
    void update();
    void render();
    void shutdown();

private:
    // Animación del spritesheet
    int arbCols = 4;          // columnas en el spritesheet
    int arbRows = 4;          // filas en el spritesheet
    int arbFrameW = 0;        // ancho de cada frame
    int arbFrameH = 0;        // alto de cada frame
    int arbTotalFrames = 0;   // arbCols * arbRows (o el total real si es menor)
    int arbFrame = 0;         // frame actual [0..arbTotalFrames-1]
    float arbFps = 8.0f;      // velocidad (frames por segundo)
    float arbAcc = 0.0f;      // acumulador de tiempo para avanzar frames
    Vector2 arbPos{ 300, 100 }; // posición donde dibujar
    float arbScale = 0.5f;    // escala para dibujar grande
};

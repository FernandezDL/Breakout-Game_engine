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
    int arbCols = 4;            // columnas en el spritesheet
    int arbRows = 4;            // filas en el spritesheet
    int arbFrameW = 0;          // ancho de cada frame
    int arbFrameH = 0;          // alto de cada frame
    int arbTotalFrames = 0;     
    int arbFrame = 0;           // frame actual
    float arbFps = 8.0f;        
    float arbAcc = 0.0f;        // acumulador de tiempo para avanzar frames
    int  animStartCol = 0;      // columna inicial del rango
    int  animEndCol   = 1;      // columna final del rango
    int animRow = 0;            // fila del spritesheet
    int animCol = 0;            // columna actual dentro del rango

    Vector2 arbPos{ 300, 100 }; // posición donde dibujar
    float arbScale = 0.5f;      // escala para dibujar grande
    float   moveSpeed = 140.0f; // velocidad de movimiento en píxeles por segundo
};

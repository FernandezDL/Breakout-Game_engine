#pragma once
#include <entt/entt.hpp>
#include <raylib.h>
#include <vector>
#include "Tilemap.h"

class Scene {
public:
    entt::registry reg;

    Texture2D bg{0};
    Texture2D hero{0};

    void setup();
    void update();
    void render();
    void shutdown();

    Tilemap tilemap;

    Tilemap& getTilemap() { return tilemap; }
    const Tilemap& getTilemap() const { return tilemap; }

    void renderTilemap();

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

struct BeeEnemy {
    Vector2 pos{0,0};
    Vector2 vel{0,0};
    float   speed = 90.0f;      // px/s
    float   scale = 0.2f;

    // Anim
    int cols = 4, rows = 2;     // 2 filas (arriba: derecha, abajo: izquierda)
    int frameW = 0, frameH = 0;
    int row = 0;                // 0 = mirando derecha, 1 = mirando izquierda
    int col = 0;                // 0..3
    int startCol = 0, endCol = 3;
    float fps = 10.0f, acc = 0.0f;

    // Cambio de dirección aleatorio
    float changeEvery = 1.0f;   // cada ~1s
    float changeTimer = 0.0f;
    };

private:
    Texture2D bee{0};
    std::vector<BeeEnemy> bees;

    // Helpers
    void spawnBee(Vector2 p);
    void updateBee(BeeEnemy& b, float dt);
    void renderBee(const BeeEnemy& b);
};
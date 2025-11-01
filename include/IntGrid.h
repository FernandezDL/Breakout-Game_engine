#pragma once
#include <vector>
#include <cstdint>
#include <raylib.h>

struct IntGrid {
    int width = 0;
    int height = 0;
    std::vector<uint8_t> cells;   // 0 = bloqueado, 1 = caminable
    bool visible = true;          // mostrar/ocultar overlay
    bool loaded = false;

    // Colores del overlay
    Color walkableColor = {  0, 255,   0, 80 }; // verde translúcido
    Color blockedColor  = {255,   0,   0, 80 }; // rojo translúcido
    Color gridLineColor = {255, 255, 255, 40 }; // líneas de la grilla

    inline bool inBounds(int x, int y) const {
        return (x >= 0 && x < width && y >= 0 && y < height);
    }

    inline uint8_t get(int x, int y) const {
        if (!inBounds(x,y)) return 0;
        return cells[y*width + x];
    }

    inline void set(int x, int y, uint8_t v) {
        if (!inBounds(x,y)) return;
        cells[y*width + x] = (v ? 1 : 0);
    }

    inline bool isWalkable(int x, int y) const { return get(x,y) == 1; }

    void resize(int w, int h, uint8_t fill = 0) {
        width = w; height = h;
        cells.assign(width*height, fill);
        loaded = true;
    }
};

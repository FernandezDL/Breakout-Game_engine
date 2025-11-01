#pragma once
#include <vector>
#include <cstdint>
#include <raylib.h>

enum class CellType : uint8_t {
    Blocked  = 0, // no caminable (pared)
    Walkable = 1, // caminable normal
    Hazard   = 2, // daña al jugador
    Slow     = 6, // piso lento
};

struct IntGrid {
    int width = 0;
    int height = 0;
    std::vector<uint8_t> cells; 
    bool visible = true;
    bool loaded = false;

    // Colores del overlay por tipo
    Color colBlocked  = {255,   0,   0, 90};
    Color colWalkable = {  0, 255,   0, 70};
    Color colHazard   = {255,  85,   0, 90};  // naranja
    Color colSlow     = {  0, 200, 255, 90};  // celeste
    Color gridLineColor = {255, 255, 255, 40};

    inline bool inBounds(int x, int y) const {
        return (x >= 0 && x < width && y >= 0 && y < height);
    }

    inline CellType get(int x, int y) const {
        if (!inBounds(x,y)) return CellType::Blocked;
        return (CellType)cells[y*width + x];
    }

    inline void set(int x, int y, CellType t) {
        if (!inBounds(x,y)) return;
        cells[y*width + x] = (uint8_t)t;
    }

    inline bool isWalkable(int x, int y) const {
        auto t = get(x,y);
        return (t == CellType::Walkable || t == CellType::Hazard || t == CellType::Slow);
    }

    void resize(int w, int h, CellType fill = CellType::Walkable) {
        width = w; height = h;
        cells.assign(width*height, (uint8_t)fill);
        loaded = true;
    }
};

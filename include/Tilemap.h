#pragma once

#include "raylib.h"
#include <vector>

struct Tilemap {
    Texture2D tileset = {0};
    int tileW = 32, tileH = 32;
    int width = 0, height = 0;   
    int tilesetCols = 4;         
    std::vector<int> tiles;
    bool loaded = false;

    inline int get(int x, int y) const {
        if (x < 0 || y < 0 || x >= width || y >= height) return 0;
        return tiles[y * width + x];
    }
};
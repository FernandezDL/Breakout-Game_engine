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
};
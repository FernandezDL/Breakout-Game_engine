#include "Systems/TilemapLoaderSystem.h"
#include "raylib.h"
#include <fstream>
#include <sstream>
#include "Scene/Scene.h"

static bool LoadCSV(const std::string& path, int& W, int& H, std::vector<int>& out) {
    std::ifstream f(path);
    if (!f) return false;

    std::string line;
    std::vector<int> data;
    int width = -1, h = 0;

    while (std::getline(f, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string cell;
        int w = 0;
        while (std::getline(ss, cell, ',')) {
            data.push_back(std::stoi(cell));
            w++;
        }
        if (width < 0) width = w;
        h++;
    }

    if (width <= 0 || h <= 0) return false;
    W = width;
    H = h;
    out.swap(data);
    return true;
}

void TilemapLoaderSystem::update() {
    if (done || !scene) return;

    auto& tm = scene->getTilemap();

    tm.tileset = LoadTexture(".\\assets\\tiles\\tiles.png");
    if (tm.tileset.id == 0) {
        TraceLog(LOG_ERROR, "No pude cargar tileset");
        done = true; 
        return;
    }

    SetTextureFilter(tm.tileset, TEXTURE_FILTER_POINT);
    tm.tileW = tm.tileH = 32;
    tm.tilesetCols = tm.tileset.width / tm.tileW;

    int W=0, H=0; std::vector<int> tiles;
    if (!LoadCSV("assets/maps/level1.csv", W, H, tiles)) {
        TraceLog(LOG_ERROR, "No pude leer assets/maps/level1.csv");
        done = true;
        return;
    }

    tm.width  = W;
    tm.height = H;
    tm.tiles  = std::move(tiles);
    tm.loaded = true;

    TraceLog(LOG_INFO, "Tilemap cargado (%dx%d)", W, H);
    done = true;
}
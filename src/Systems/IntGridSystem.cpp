#include "Systems/IntGridSystem.h"
#include "Scene/Scene.h"
#include "raylib.h"
#include <fstream>
#include <sstream>

// Helpers CSV simples (mismo tamaño que el Tilemap)
static bool LoadIntGridCSV(const std::string& path, int& W, int& H, std::vector<uint8_t>& out) {
    std::ifstream f(path);
    if (!f) return false;

    std::string line;
    std::vector<uint8_t> data;
    int width = -1, h = 0;

    while (std::getline(f, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string cell;
        int rowCount = 0;
        while (std::getline(ss, cell, ',')) {
            int v = 0;
            try { v = std::stoi(cell); } catch (...) { v = 0; }
            data.push_back((v != 0) ? 1 : 0);
            rowCount++;
        }
        if (width < 0) width = rowCount;
        else if (rowCount != width) return false; // filas desiguales
        h++;
    }

    if (width <= 0 || h <= 0) return false;
    W = width; H = h; out = std::move(data);
    return true;
}

static bool SaveIntGridCSV(const std::string& path, int W, int H, const std::vector<uint8_t>& data) {
    if ((int)data.size() != W*H) return false;
    std::ofstream f(path, std::ios::trunc);
    if (!f) return false;
    for (int y = 0; y < H; ++y) {
        for (int x = 0; x < W; ++x) {
            f << int(data[y*W + x]);
            if (x+1 < W) f << ",";
        }
        f << "\n";
    }
    return true;
}

void IntGridSystem::setup() {
    // Asegura tamaño del IntGrid = tamaño del Tilemap. Si existe CSV, cargarlo.
    auto& tm = scene->tilemap;
    auto& ig = scene->intgrid;

    if (!tm.loaded || tm.width <= 0 || tm.height <= 0) {
        TraceLog(LOG_WARNING, "IntGridSystem.setup: Tilemap no cargado todavía.");
        return;
    }

    int W = 0, H = 0;
    std::vector<uint8_t> cells;
    if (LoadIntGridCSV("assets/maps/level1.walk.csv", W, H, cells) && W == tm.width && H == tm.height) {
        ig.width = W; ig.height = H; ig.cells = std::move(cells); ig.loaded = true;
        TraceLog(LOG_INFO, "IntGrid cargado de assets/maps/level1.walk.csv (%dx%d)", W, H);
    } else {
        ig.resize(tm.width, tm.height, /*fill=*/1); // por defecto TODO caminable
        TraceLog(LOG_INFO, "IntGrid creado por defecto (todo caminable) (%dx%d)", ig.width, ig.height);
    }
}

void IntGridSystem::update() {
    auto& tm = scene->tilemap;
    auto& ig = scene->intgrid;
    if (!tm.loaded || !ig.loaded) return;

    // Toggle visibilidad con tecla G
    if (IsKeyPressed(KEY_G)) {
        ig.visible = !ig.visible;
    }

    // Guardar CSV con tecla F5
    if (IsKeyPressed(KEY_F5)) {
        if (SaveIntGridCSV("assets/maps/level1.walk.csv", ig.width, ig.height, ig.cells))
            TraceLog(LOG_INFO, "IntGrid guardado en assets/maps/level1.walk.csv");
        else
            TraceLog(LOG_ERROR, "No se pudo guardar assets/maps/level1.walk.csv");
    }

    // Limpiar todo a 1 (caminable) con SHIFT + C
    if (IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_C)) {
        for (auto& v : ig.cells) v = 1;
        TraceLog(LOG_INFO, "IntGrid: todo caminable.");
    }
    // Limpiar todo a 0 (bloqueado) con CTRL + C
    if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_C)) {
        for (auto& v : ig.cells) v = 0;
        TraceLog(LOG_INFO, "IntGrid: todo bloqueado.");
    }

    // Edición con mouse (izquierdo = poner caminable=1, derecho = 0)
    Vector2 m = GetMousePosition();
    int cx = (int)(m.x / tm.tileW);
    int cy = (int)(m.y / tm.tileH);
    if (ig.inBounds(cx, cy)) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))  ig.set(cx, cy, 1);
        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) ig.set(cx, cy, 0);
    }
}

void IntGridSystem::render() {
    auto& tm = scene->tilemap;
    auto& ig = scene->intgrid;
    if (!tm.loaded || !ig.loaded || !ig.visible) return;

    // Overlay por celda
    for (int y = 0; y < ig.height; ++y) {
        for (int x = 0; x < ig.width; ++x) {
            Rectangle r{ (float)(x*tm.tileW), (float)(y*tm.tileH), (float)tm.tileW, (float)tm.tileH };
            uint8_t v = ig.get(x,y);
            DrawRectangleRec(r, v ? ig.walkableColor : ig.blockedColor);
        }
    }

    // Líneas de grilla (opcionales)
    for (int x = 0; x <= ig.width; ++x) {
        DrawLine(x*tm.tileW, 0, x*tm.tileW, ig.height*tm.tileH, ig.gridLineColor);
    }
    for (int y = 0; y <= ig.height; ++y) {
        DrawLine(0, y*tm.tileH, ig.width*tm.tileW, y*tm.tileH, ig.gridLineColor);
    }
}

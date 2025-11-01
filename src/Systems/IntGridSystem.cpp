#include "Systems/IntGridSystem.h"
#include "Scene/Scene.h"
#include "raylib.h"
#include <fstream>
#include <sstream>

static bool LoadIntGridCSV(const std::string& path, int& W, int& H, std::vector<uint8_t>& out) {
    std::ifstream f(path);
    if (!f) return false;
    std::string line;
    std::vector<uint8_t> data;
    int width = -1, h = 0;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string cell; int rowCount = 0;
        while (std::getline(ss, cell, ',')) {
            int v = 0; try { v = std::stoi(cell); } catch (...) { v = 0; }
            if (v < 0) v = 0; if (v > 6) v = 6; // clamp 0..6
            data.push_back((uint8_t)v);
            rowCount++;
        }
        if (width < 0) width = rowCount; else if (rowCount != width) return false;
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

static Color colorFor(CellType t, const IntGrid& ig) {
    switch (t) {
        case CellType::Blocked:  return ig.colBlocked;
        case CellType::Walkable: return ig.colWalkable;
        case CellType::Hazard:   return ig.colHazard;
        case CellType::Slow:     return ig.colSlow;
        default: return ig.colWalkable;
    }
}

void IntGridSystem::setup() {
    auto& tm = scene->tilemap;
    auto& ig = scene->intgrid;
    if (!tm.loaded || tm.width <= 0 || tm.height <= 0) {
        TraceLog(LOG_WARNING, "IntGridSystem.setup: Tilemap no cargado.");
        return;
    }

    int W=0,H=0; std::vector<uint8_t> cells;
    if (LoadIntGridCSV("assets/maps/level1.intgrid.csv", W, H, cells) && W==tm.width && H==tm.height) {
        ig.width = W; ig.height = H; ig.cells = std::move(cells); ig.loaded = true;
        TraceLog(LOG_INFO, "IntGrid cargado (tipos) %dx%d", W, H);
    } else {
        ig.resize(tm.width, tm.height, CellType::Walkable);
        TraceLog(LOG_INFO, "IntGrid creado por defecto (Walkable) %dx%d", ig.width, ig.height);
    }
}

void IntGridSystem::update() {
    auto& tm = scene->tilemap;
    auto& ig = scene->intgrid;
    if (!tm.loaded || !ig.loaded) return;

    // Toggle visibilidad
    if (IsKeyPressed(KEY_G)) ig.visible = !ig.visible;

    // Guardar CSV
    if (IsKeyPressed(KEY_F5)) {
        if (SaveIntGridCSV("assets/maps/level1.intgrid.csv", ig.width, ig.height, ig.cells))
            TraceLog(LOG_INFO, "IntGrid guardado (assets/maps/level1.intgrid.csv)");
        else
            TraceLog(LOG_ERROR, "No se pudo guardar level1.intgrid.csv");
    }

    static CellType brush = CellType::Walkable;
    if (IsKeyPressed(KEY_ZERO)) brush = CellType::Blocked;
    if (IsKeyPressed(KEY_ONE))  brush = CellType::Walkable;
    if (IsKeyPressed(KEY_TWO))  brush = CellType::Hazard;
    if (IsKeyPressed(KEY_SIX))  brush = CellType::Slow;

    // Pintar con el mouse
    Vector2 m = GetMousePosition();
    int cx = (int)(m.x / tm.tileW);
    int cy = (int)(m.y / tm.tileH);
    if (ig.inBounds(cx, cy)) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))  ig.set(cx, cy, brush);
        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) ig.set(cx, cy, CellType::Blocked);
    }
}

void IntGridSystem::render() {
    auto& tm = scene->tilemap;
    auto& ig = scene->intgrid;
    if (!tm.loaded || !ig.loaded || !ig.visible) return;

    // Celdas
    for (int y = 0; y < ig.height; ++y) {
        for (int x = 0; x < ig.width; ++x) {
            Rectangle r{ (float)(x*tm.tileW), (float)(y*tm.tileH), (float)tm.tileW, (float)tm.tileH };
            auto t = ig.get(x,y);
            DrawRectangleRec(r, colorFor(t, ig));
        }
    }
    // Líneas
    for (int x = 0; x <= ig.width; ++x)
        DrawLine(x*tm.tileW, 0, x*tm.tileW, ig.height*tm.tileH, ig.gridLineColor);
    for (int y = 0; y <= ig.height; ++y)
        DrawLine(0, y*tm.tileH, ig.width*tm.tileW, y*tm.tileH, ig.gridLineColor);

    // HUD del brush
    DrawText("Brush: 0=Blocked, 1=Walk, 2=Hazard, 3=Slow", 10, 40, 14, WHITE);
}

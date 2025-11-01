#pragma once
#include "Scene/Scene.h"
#include <raylib.h>
#include <cmath>

inline bool gridCellWalkable(const Scene* scene, int cx, int cy) {
    const auto& ig = scene->intgrid;
    if (!ig.inBounds(cx, cy)) return false;
    return ig.isWalkable(cx, cy);
}

inline bool rectWalkableOnGrid(const Scene* scene, Rectangle r) {
    const float inset = 1.0f; 
    const auto& tm = scene->tilemap;

    Vector2 pts[4] = {
        {r.x + inset,             r.y + inset},
        {r.x + r.width - inset,   r.y + inset},
        {r.x + inset,             r.y + r.height - inset},
        {r.x + r.width - inset,   r.y + r.height - inset}
    };
    for (auto& p : pts) {
        int cx = (int)std::floor(p.x / tm.tileW);
        int cy = (int)std::floor(p.y / tm.tileH);
        if (!gridCellWalkable(scene, cx, cy)) return false;
    }
    return true;
}

inline Vector2 moveWithGrid(const Scene* scene, Rectangle actorRect, Vector2 delta) {
    Rectangle r = actorRect;

    // Eje X
    r.x += delta.x;
    if (!rectWalkableOnGrid(scene, r)) {
        r.x -= delta.x;
        delta.x = 0;
    }

    // Eje Y
    r.y += delta.y;
    if (!rectWalkableOnGrid(scene, r)) {
        r.y -= delta.y;
        delta.y = 0;
    }
    return delta;
}

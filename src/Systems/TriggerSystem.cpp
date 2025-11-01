#include "Scene/Scene.h"
#include <raylib.h>
#include <algorithm>

struct PlayerState {
    int hp = 100;
    int keys = 0;
};
static PlayerState gPlayer; // simplificado (muévelo a Scene si prefieres)

void TriggerSystem_Update(Scene* scene, float dt) {
    auto& ig = scene->intgrid;
    auto& tm = scene->tilemap;

    // Celda bajo el centro del jugador
    Vector2 arb = scene->getArbPos();
    int cx = (int)std::floor(arb.x / tm.tileW);
    int cy = (int)std::floor(arb.y / tm.tileH);
    if (!ig.inBounds(cx, cy)) return;

    CellType t = ig.get(cx, cy);
    switch (t) {
        case CellType::Hazard:
            // daño por segundo
            gPlayer.hp -= (int)(15 * dt);
            if (gPlayer.hp < 0) gPlayer.hp = 0;
            break;

        default: break;
    }
}

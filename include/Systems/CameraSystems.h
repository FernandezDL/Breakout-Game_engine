#pragma once
#include <entt/entt.hpp>
#include "raylib.h"
#include "components/Transform2D.h"
#include "components/Camera2DComponent.h"
#include "components/Tags.h"

// Inicializa la cámara con valores razonables
struct CameraSetupSystem {
    void operator()(entt::registry& reg, Rectangle worldBounds) const;
};

// Sigue al jugador (Tag: Player) con suavizado y clamp a world
struct CameraFollowSystem {
    void operator()(entt::registry& reg, float dt) const;
};

// Sacudida (shake) de cámara como “effect system”
struct CameraShakeSystem {
    void trigger(entt::registry& reg, float duration, float strength) const;
    void update(entt::registry& reg, float dt) const;
};

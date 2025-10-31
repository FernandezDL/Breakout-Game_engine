#include "systems/CameraSystems.h"
#include <cmath>

static inline float lerp(float a, float b, float t) { return a + (b - a) * t; }

void CameraSetupSystem::operator()(entt::registry& reg, Rectangle worldBounds) const {
    auto camEnt = reg.create();
    auto& camc = reg.emplace<Camera2DComponent>(camEnt);
    camc.worldBounds = worldBounds;
    camc.zoom = 1.0f;
    camc.offset = { (float)GetScreenWidth() * 0.5f, (float)GetScreenHeight() * 0.5f };

    camc.cam.target = { 0, 0 };
    camc.cam.offset = camc.offset;
    camc.cam.rotation = 0.0f;
    camc.cam.zoom = camc.zoom;

    // Marca única si quieres localizarlo rápido
    reg.emplace<TagCamera>(camEnt);
}

void CameraFollowSystem::operator()(entt::registry& reg, float dt) const {
    // ---- localizar la cámara (TagCamera) sin iteradores ----
    auto camView = reg.view<Camera2DComponent, TagCamera>();

    entt::entity camEnt = entt::null;
    Camera2DComponent* camPtr = nullptr;

    camView.each([&](entt::entity e, Camera2DComponent &c) {
        if (camEnt == entt::null) { camEnt = e; camPtr = &c; }
    });
    if (camEnt == entt::null || camPtr == nullptr) return;

    auto& camc = *camPtr;

    // ---- localizar Player  ----
    auto playerView = reg.view<Transform2D, PlayerTag>();

    entt::entity playerEnt = entt::null;
    Transform2D* tPtr = nullptr;

    playerView.each([&](entt::entity e, Transform2D &t) {
        if (playerEnt == entt::null) { playerEnt = e; tPtr = &t; }
    });
    if (playerEnt == entt::null || tPtr == nullptr) return;

    Vector2 targetPos{ tPtr->position.x, tPtr->position.y };

    // ---- suavizado exponencial ----
    float k = 1.0f - std::exp(-camc.lerp * dt);
    camc.cam.target.x = lerp(camc.cam.target.x, targetPos.x, k);
    camc.cam.target.y = lerp(camc.cam.target.y, targetPos.y, k);

    // ---- clamp a límites del mundo ----
    float halfW = (GetScreenWidth()  * 0.5f) / camc.cam.zoom;
    float halfH = (GetScreenHeight() * 0.5f) / camc.cam.zoom;

    float minX = camc.worldBounds.x + halfW;
    float maxX = camc.worldBounds.x + camc.worldBounds.width  - halfW;
    float minY = camc.worldBounds.y + halfH;
    float maxY = camc.worldBounds.y + camc.worldBounds.height - halfH;

    camc.cam.target.x = std::fmax(minX, std::fmin(maxX, camc.cam.target.x));
    camc.cam.target.y = std::fmax(minY, std::fmin(maxY, camc.cam.target.y));

    // ---- aplicar zoom/offset ----
    camc.cam.zoom   = camc.zoom;
    camc.cam.offset = camc.offset;
}

void CameraShakeSystem::trigger(entt::registry& reg, float duration, float strength) const {
    auto camView = reg.view<Camera2DComponent, TagCamera>();

    entt::entity camEnt = entt::null;
    Camera2DComponent* camPtr = nullptr;

    camView.each([&](entt::entity e, Camera2DComponent &c) {
        if (camEnt == entt::null) { camEnt = e; camPtr = &c; }
    });
    if (camEnt == entt::null || camPtr == nullptr) return;

    camPtr->shakeTime     = duration;
    camPtr->shakeStrength = strength;
}

void CameraShakeSystem::update(entt::registry& reg, float dt) const {
    auto camView = reg.view<Camera2DComponent, TagCamera>();

    entt::entity camEnt = entt::null;
    Camera2DComponent* camPtr = nullptr;

    camView.each([&](entt::entity e, Camera2DComponent &c) {
        if (camEnt == entt::null) { camEnt = e; camPtr = &c; }
    });
    if (camEnt == entt::null || camPtr == nullptr) return;

    auto& camc = *camPtr;

    if (camc.shakeTime > 0.0f) {
        camc.shakeTime -= dt;

        float rx = (float)(GetRandomValue(-1000, 1000)) / 1000.0f;
        float ry = (float)(GetRandomValue(-1000, 1000)) / 1000.0f;
        Vector2 jitter{ rx * camc.shakeStrength, ry * camc.shakeStrength };

        camc.cam.offset = { camc.offset.x + jitter.x, camc.offset.y + jitter.y };

        if (camc.shakeTime <= 0.0f) {
            camc.cam.offset = camc.offset;
        }
    }
}

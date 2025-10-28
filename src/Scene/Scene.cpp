#include "Scene/Scene.h"
#include "Components/Transform2D.h"
#include "Components/Sprite.h"
#include "Components/Animator.h"
#include "Components/Input.h"
#include "Components/Tags.h"

static void system_input(entt::registry& reg, float dt) {
    auto view = reg.view<Transform2D, InputControlled, PlayerTag>();
    for (auto e : view) {
        auto& t = view.get<Transform2D>(e);
        t.velocity = {0,0};
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  t.velocity.x = -t.speed;
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) t.velocity.x =  t.speed;
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    t.velocity.y = -t.speed;
        if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  t.velocity.y =  t.speed;
    }
}

static void system_movement(entt::registry& reg, float dt) {
    auto view = reg.view<Transform2D>();
    for (auto e : view) {
        auto& t = view.get<Transform2D>(e);
        t.position.x += t.velocity.x * dt;
        t.position.y += t.velocity.y * dt;
    }
}

static void system_animation(entt::registry& reg, float dt) {
    auto view = reg.view<Animator, Sprite, Transform2D>();
    for (auto e : view) {
        auto& a = view.get<Animator>(e);
        auto& s = view.get<Sprite>(e);
        auto& t = view.get<Transform2D>(e);

        // decidir estado por velocidad
        std::string state;
        if (t.velocity.x == 0 && t.velocity.y == 0) {
            // idle: preserva la última dirección (a.state)
            if (a.state.rfind("walk_", 0) == 0)
                a.state.replace(0, 4, "idle"); // walk_* -> idle_*
            state = a.state;
        } else {
            // dirección dominante
            if (std::abs(t.velocity.x) > std::abs(t.velocity.y))
                state = (t.velocity.x > 0) ? "walk_right" : "walk_left";
            else
                state = (t.velocity.y > 0) ? "walk_down" : "walk_up";
        }

        // si cambió de anim, resetea contador
        if (state != a.state) {
            a.state = state;
            a.timeAcc = 0.f;
            a.current = a.clips[state].startFrame;
        }

        // avanzar frames
        const auto& clip = a.clips[a.state];
        a.timeAcc += dt;
        const float frameDur = 1.0f / clip.fps;
        while (a.timeAcc >= frameDur) {
            a.timeAcc -= frameDur;
            int localIndex = ( (a.current - clip.startFrame) + 1 ) % clip.length;
            a.current = clip.startFrame + localIndex;
        }

        // actualizar rect fuente
        int col = a.current % a.columns;
        int row = a.current / a.columns;
        s.src = { (float)(col * a.frameWidth),
                  (float)(row * a.frameHeight),
                  (float)a.frameWidth, (float)a.frameHeight };
        s.currentAnim = a.state;
    }
}

static void system_render(entt::registry& reg, Texture2D& bg) {
    BeginDrawing();
    DrawTexture(bg, 0, 0, WHITE);

    auto view = reg.view<Sprite, Transform2D>();
    for (auto e : view) {
        auto& s = view.get<Sprite>(e);
        auto& t = view.get<Transform2D>(e);
        DrawTexturePro(
            *s.texture,
            s.src,
            Rectangle{ t.position.x, t.position.y, s.src.width * s.scale, s.src.height * s.scale },
            s.origin,
            0.0f,
            WHITE
        );
    }

    EndDrawing();
}

void Scene::setup() {
    bg = LoadTexture(".\\assets\\background\\grass.jpeg");  
    hero = LoadTexture(".\\assets\\sprites\\arbol.png");  

    if (hero.id == 0) TraceLog(LOG_ERROR, "HERO NOT LOADED");
    if (bg.id == 0) TraceLog(LOG_ERROR, "BACKGROUND NOT LOADED");
}

void Scene::update() {
    const float dt = GetFrameTime();
    system_input(reg, dt);
    system_movement(reg, dt);
    system_animation(reg, dt);
}

void Scene::render() {
    // 1) Fondo (escala a tamaño de ventana)
    if (bg.id) {
        const int sw = GetScreenWidth();
        const int sh = GetScreenHeight();
        const Rectangle src = { 0, 0, (float)bg.width, (float)bg.height };
        const Rectangle dst = { 0, 0, (float)sw, (float)sh };
        DrawTexturePro(bg, src, dst, {0,0}, 0.0f, WHITE);
    } else {
        // Fallback visible para depurar
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), DARKGREEN);
        DrawText("No se cargo el fondo", 20, 20, 20, RED);
    }

    // 2) “Personaje”/sprite simple (prueba de dibujo)
    if (hero.id) {
        DrawTexture(hero, 100, 100, WHITE);
    } else {
        DrawText("No se cargo el hero", 20, 50, 20, RED);
    }
}

void Scene::shutdown() {
    UnloadTexture(bg);
    UnloadTexture(hero);
    // UnloadTexture(slime);
}

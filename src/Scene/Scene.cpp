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

        std::string state;
        if (t.velocity.x == 0 && t.velocity.y == 0) {
            if (a.state.rfind("walk_", 0) == 0)
                a.state.replace(0, 4, "idle");
            state = a.state;
        } else {
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

    arbCols = 4;
    arbRows = 4;
    arbTotalFrames = arbCols * arbRows;

    arbFrameW = hero.width  / arbCols;
    arbFrameH = hero.height / arbRows;

    arbFps = 8.0f;
    arbAcc = 0.0f;
    arbFrame = 0;
    animStartCol = 0;    
    animEndCol   = 1;
    animRow = 0;
    animCol = animStartCol;

    arbPos = { 300, 100 };
    arbScale = 0.5f;
    moveSpeed = 140.0f;
}

void Scene::update() {
    float dt = GetFrameTime();

    // --- INPUT & MOVIMIENTO ---
    float vx = 0.0f, vy = 0.0f;
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) vx += 1.0f;
    if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) vx -= 1.0f;
    if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) vy += 1.0f;
    if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) vy -= 1.0f;

    bool moving = (vx != 0.0f || vy != 0.0f);

    // Normaliza diagonal
    if (moving) {
        float len = sqrtf(vx*vx + vy*vy);
        vx /= len; vy /= len;
    }

    // Mover
    arbPos.x += vx * moveSpeed * dt;
    arbPos.y += vy * moveSpeed * dt;

    // Mantener dentro de pantalla
    float w = arbFrameW * arbScale;
    float h = arbFrameH * arbScale;
    float minX = w * 0.5f, maxX = GetScreenWidth()  - w * 0.5f;
    float minY = h * 0.5f, maxY = GetScreenHeight() - h * 0.5f;
    if (arbPos.x < minX) arbPos.x = minX;
    if (arbPos.x > maxX) arbPos.x = maxX;
    if (arbPos.y < minY) arbPos.y = minY;
    if (arbPos.y > maxY) arbPos.y = maxY;

    // --- ANIMACIÓN ---
    int newRow        = animRow;
    int newStartCol   = animStartCol;
    int newEndCol     = animEndCol;
    float newFps      = arbFps;

    if (!moving) {
        // IDLE: fila 0, columnas 0..1
        newRow = 0; newStartCol = 0; newEndCol = 1; newFps = 4.0f;
    } else {
        // WALK: usa la fila por dirección
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))      newRow = 2;
        else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))  newRow = 1;
        else                                               newRow = 3;
        newStartCol = 0; newEndCol = 3; newFps = 10.0f;
    }

    // Si cambió el rango (fila o columnas), resetea ciclo
    bool rangeChanged = (newRow != animRow) ||
                        (newStartCol != animStartCol) ||
                        (newEndCol != animEndCol);

    if (rangeChanged) {
        animRow = newRow;
        animStartCol = newStartCol;
        animEndCol   = newEndCol;
        arbFps = newFps;
        animCol = animStartCol;   // reinicia al primer frame del rango
        arbAcc = 0.0f;            // reinicia temporizador
    } else {
        arbFps = newFps;
    }

    // --- Avance de frames por tiempo ---
    arbAcc += dt;
    const float step = 1.0f / arbFps;
    while (arbAcc >= step) {
        arbAcc -= step;
        animCol++;
        if (animCol > animEndCol) animCol = animStartCol;
    }
}

void Scene::render() {
    // Fondo
    if (bg.id) {
        const int sw = GetScreenWidth();
        const int sh = GetScreenHeight();
        const Rectangle src = { 0, 0, (float)bg.width, (float)bg.height };
        const Rectangle dst = { 0, 0, (float)sw, (float)sh };
        DrawTexturePro(bg, src, dst, {0,0}, 0.0f, WHITE);
    } else {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), DARKGREEN);
        DrawText("No se cargo el fondo", 20, 20, 20, RED);
    }

    // Sprite animado
    if (hero.id) {
        Rectangle src{
            (float)(animCol * arbFrameW),
            (float)(animRow * arbFrameH),
            (float)arbFrameW,
            (float)arbFrameH
        };

        float w = arbFrameW * arbScale;
        float h = arbFrameH * arbScale;

        Rectangle dst{ arbPos.x, arbPos.y, w, h };
        Vector2 origin{ w/2.0f, h/2.0f };  

        DrawTexturePro(hero, src, dst, origin, 0.0f, WHITE);
    }
}

void Scene::shutdown() {
    UnloadTexture(bg);
    UnloadTexture(hero);
    // UnloadTexture(slime);
}

#include "Scene/Scene.h"
#include "Components/Transform2D.h"
#include "Components/Sprite.h"
#include "Components/Animator.h"
#include "Components/Input.h"
#include "Components/Tags.h"
#include "Systems/TilemapLoaderSystem.h"
#include "Tilemap.h"
#include <fstream>      
#include <sstream>     
#include <string>      
#include <vector>
#include "Systems/IntGridSystem.h"

static bool LoadCSV(const std::string& path, int& W, int& H, std::vector<int>& out) {
    std::ifstream f(path);
    if (!f) return false;

    std::string line;
    std::vector<int> data;
    int width = -1, h = 0;

    while (std::getline(f, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string cell; int w = 0;
        while (std::getline(ss, cell, ',')) {
            data.push_back(std::stoi(cell));
            w++;
        }
        if (width < 0) width = w;
        h++;
    }
    if (width <= 0 || h <= 0) return false;
    W = width; H = h; out.swap(data);
    return true;
}

static Rectangle IndexToSrc(int idx, int tileW, int tileH, int cols, int spacing = 0, int margin = 0) {
    int tx = idx % cols;
    int ty = idx / cols;

    float x = margin + tx * (tileW + spacing);
    float y = margin + ty * (tileH + spacing);

    const float eps = 0.01f;
    return { x + eps, y + eps, tileW - 2*eps, tileH - 2*eps };
}

static Rectangle SrcFromIndex4x4(int idx, const Texture2D& tex) {
    const int cols = 4, rows = 4;
    const int tileW = tex.width  / cols;
    const int tileH = tex.height / rows;

    int tx = idx % cols;   
    int ty = idx / cols; 

    float x = (float)(tx * tileW);
    float y = (float)(ty * tileH);
    return { x, y, (float)tileW, (float)tileH };
}

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
    hero = LoadTexture(".\\assets\\sprites\\arbol.png");  
    bee = LoadTexture(".\\assets\\sprites\\abeja.png");

    if (hero.id == 0) TraceLog(LOG_ERROR, "HERO NOT LOADED");
    if (bee.id == 0) TraceLog(LOG_ERROR, "BEE NOT LOADED");

    TilemapLoaderSystem loader;
    loader.setScene(this);
    loader.update();

    IntGridSystem igsys;
    igsys.setScene(this);
    igsys.setup();

    SetTextureFilter(bee, TEXTURE_FILTER_POINT);

    if (bee.id == 0) TraceLog(LOG_ERROR, "BEE NOT LOADED");

    if (!bee.id) {
        TraceLog(LOG_ERROR, "BEE NOT LOADED");
    } else {
        // spawnea 2 abejas en posiciones aleatorias visibles
        for (int i = 0; i < 2; ++i) {
            float x = (float)GetRandomValue(80, GetScreenWidth()  - 80);
            float y = (float)GetRandomValue(80, GetScreenHeight() - 80);
            spawnBee({x, y});
        }
        TraceLog(LOG_INFO, "Spawned %d bees", (int)bees.size());
    }

    // Arbol
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

    // Abeja
    BeeEnemy b;
    b.cols = 4;
    b.rows = 2;
    b.frameW = bee.width  / b.cols; 
    b.frameH = bee.height / b.rows;
    b.startCol = 0;
    b.endCol   = 3; 
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

    if (!moving) { // IDLE
        newRow = 0; newStartCol = 0; newEndCol = 1; newFps = 4.0f;
    } else { // WALK
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))      newRow = 2;
        else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))  newRow = 1;
        else                                               newRow = 3;
        newStartCol = 0; newEndCol = 3; newFps = 10.0f;
    }

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

    // --- ACTUALIZAR ABEJAS ---
    for (auto& b : bees) updateBee(b, dt);

    static IntGridSystem igsys;
    igsys.setScene(this);
    igsys.update();
}

void Scene::render() {
    // Fondo
    renderTilemap();

    // IntGrid
    static IntGridSystem igsys;
    igsys.setScene(this);
    igsys.render();

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

    // Abejas
    for (const auto& b : bees) renderBee(b);
}

void Scene::shutdown() {
    // UnloadTexture(bg);
    UnloadTexture(hero);
    UnloadTexture(bee);
}

void Scene::spawnBee(Vector2 p) {
    BeeEnemy b;
    b.pos = p;

    b.frameW = bee.width  / b.cols; 
    b.frameH = bee.height / b.rows; 

    float ang = GetRandomValue(0, 628) / 100.0f;
    b.vel = { cosf(ang)*b.speed, sinf(ang)*b.speed };
    b.row = (b.vel.x >= 0) ? 0 : 1;

    b.changeEvery = GetRandomValue(60, 150) / 100.0f;

    bees.push_back(b);
}

void Scene::updateBee(BeeEnemy& b, float dt) {
    // cambio de rumbo aleatorio
    b.changeTimer += dt;
    if (b.changeTimer >= b.changeEvery) {
        b.changeTimer = 0.0f;
        b.changeEvery = GetRandomValue(60, 150) / 100.0f;
        float ang = GetRandomValue(0, 628) / 100.0f;
        b.vel = { cosf(ang)*b.speed, sinf(ang)*b.speed };
    }

    // mover
    b.pos.x += b.vel.x * dt;
    b.pos.y += b.vel.y * dt;

    // rebotar en bordes (anclado al centro)
    float w = b.frameW * b.scale, h = b.frameH * b.scale;
    float minX = w*0.5f, maxX = GetScreenWidth()  - w*0.5f;
    float minY = h*0.5f, maxY = GetScreenHeight() - h*0.5f;
    if (b.pos.x < minX) { b.pos.x = minX; b.vel.x = fabsf(b.vel.x); }
    if (b.pos.x > maxX) { b.pos.x = maxX; b.vel.x = -fabsf(b.vel.x); }
    if (b.pos.y < minY) { b.pos.y = minY; b.vel.y = fabsf(b.vel.y); }
    if (b.pos.y > maxY) { b.pos.y = maxY; b.vel.y = -fabsf(b.vel.y); }

    // fila según dirección horizontal
    b.row = (b.vel.x >= 0) ? 0 : 1;

    // animación 0..3 (vuelo continuo)
    b.acc += dt;
    const float step = 1.0f / b.fps;
    while (b.acc >= step) {
        b.acc -= step;
        b.col = (b.col + 1) > b.endCol ? b.startCol : (b.col + 1);
    }
}

void Scene::renderBee(const BeeEnemy& b) {
    if (!bee.id) {
        DrawCircleV(b.pos, 10, RED);
        return;
    }
    Rectangle src{
        (float)(b.col * b.frameW),
        (float)(b.row * b.frameH),
        (float)b.frameW,
        (float)b.frameH
    };
    float w = b.frameW * b.scale, h = b.frameH * b.scale;
    Rectangle dst{ b.pos.x, b.pos.y, w, h };
    Vector2 origin{ w/2.0f, h/2.0f };
    DrawTexturePro(bee, src, dst, origin, 0.0f, WHITE);
}

void Scene::renderTilemap() {
    const Tilemap& tm = tilemap;
    if (!tm.loaded || tm.tileset.id == 0) return;

    for (int y = 0; y < tm.height; ++y) {
        for (int x = 0; x < tm.width; ++x) {
            int raw = tm.tiles[y * tm.width + x];
            int idx = (raw <= 0) ? -1 : (raw - 1);
            if (idx < 0) continue;

            Rectangle src = SrcFromIndex4x4(idx, tm.tileset);
            Rectangle dst = { (float)(x * tm.tileW), (float)(y * tm.tileH),
                              (float)tm.tileW, (float)tm.tileH };
            DrawTexturePro(tm.tileset, src, dst, {0,0}, 0, WHITE);
        }
    }
}
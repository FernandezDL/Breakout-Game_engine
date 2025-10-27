#include "Game.h"
#include "ecs/Components.h"
#include <cmath>
#include <vector>

Game::Game(const char* title, int width, int height)
: screen_width(width), screen_height(height) {
    InitWindow(screen_width, screen_height, title);
    SetTargetFPS(60);
}

Game::~Game() {}

void Game::setup() {
    // Paddle
    paddle = r.create();
    r.emplace<Transform2D>(paddle, Transform2D{ Vector2{screen_width/2.0f - 60.0f, screen_height - 40.0f}, Vector2{120.0f, 18.0f} });
    r.emplace<AABB>(paddle);
    r.emplace<RenderColor>(paddle, RenderColor{ BLUE, true });
    r.emplace<Tags::Paddle>(paddle);

    // Ball
    ball = r.create();
    r.emplace<Transform2D>(ball, Transform2D{ Vector2{screen_width/2.0f - 8.0f, screen_height/2.0f}, Vector2{16.0f, 16.0f} });
    r.emplace<Velocity>(ball,  Velocity{ Vector2{160.0f, -160.0f} });
    r.emplace<AABB>(ball);
    r.emplace<RenderColor>(ball, RenderColor{ WHITE, true });
    r.emplace<Tags::Ball>(ball);

    // Blocks grid
    const int cols = 10, rows = 6;
    const int spacing = 6;
    const Vector2 blockSize{ 96.0f, 24.0f };
    const float startX = (screen_width - (cols*blockSize.x + (cols-1)*spacing)) * 0.5f;
    const float startY = 60.0f;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            float x = startX + j * (blockSize.x + spacing);
            float y = startY + i * (blockSize.y + spacing);
            auto e = r.create();
            r.emplace<Transform2D>(e, Transform2D{ Vector2{x, y}, blockSize });
            r.emplace<AABB>(e);

            Color c = (i%2==0)? RED : ORANGE;
            if(i==2) c = YELLOW;
            if(i==3) c = GREEN;
            if(i==4) c = BLUE;
            if(i==5) c = PURPLE;

            r.emplace<RenderColor>(e, RenderColor{ c, true });
            r.emplace<Health>(e, Health{1});
            r.emplace<Tags::Block>(e);
        }
    }
}

void Game::frame_start() {
    BeginDrawing();
    ClearBackground(BLACK);
}

void Game::handle_events() {
    if (WindowShouldClose()) isRunning = false;

    // Paddle move
    const float speed = 420.0f * GetFrameTime();
    auto &pT = r.get<Transform2D>(paddle);
    if (IsKeyDown(KEY_LEFT))  pT.pos.x -= speed;
    if (IsKeyDown(KEY_RIGHT)) pT.pos.x += speed;
    pT.pos.x = std::fmax(0.0f, std::fmin(pT.pos.x, static_cast<float>(screen_width) - pT.size.x));
}

bool Game::intersects(const Rectangle& a, const Rectangle& b) const {
    return a.x < b.x + b.width && a.x + a.width > b.x &&
           a.y < b.y + b.height && a.y + a.height > b.y;
}

void Game::update() {
    float dt = GetFrameTime();

    // Move ball
    auto &bT = r.get<Transform2D>(ball);
    auto &bV = r.get<Velocity>(ball);
    bT.pos.x += bV.vel.x * dt;
    bT.pos.y += bV.vel.y * dt;

    // Walls bounce
    if (bT.pos.x <= 0) { bT.pos.x = 0; bV.vel.x *= -1; }
    if (bT.pos.x + bT.size.x >= screen_width) { bT.pos.x = screen_width - bT.size.x; bV.vel.x *= -1; }
    if (bT.pos.y <= 0) { bT.pos.y = 0; bV.vel.y *= -1; }
    if (bT.pos.y >= screen_height) {
        bT.pos = { screen_width/2.0f - 8.0f, screen_height/2.0f };
        bV.vel = { 160.0f, -160.0f };
    }

    // Paddle collision
    Rectangle ballRect{ bT.pos.x, bT.pos.y, bT.size.x, bT.size.y };
    const auto &pT = r.get<Transform2D>(paddle);
    Rectangle pRect{ pT.pos.x, pT.pos.y, pT.size.x, pT.size.y };
    if (intersects(ballRect, pRect) && bV.vel.y > 0) {
        bT.pos.y = pT.pos.y - bT.size.y;
        bV.vel.y *= -1;
        float center = pT.pos.x + pT.size.x * 0.5f;
        float dist   = (bT.pos.x + bT.size.x*0.5f) - center;
        bV.vel.x += dist * 3.0f;
    }

    // Blocks collision & destroy
    auto blocks = r.view<Tags::Block>();
    for (auto e : blocks) {
        if (!r.all_of<Transform2D, AABB, RenderColor, Health>(e)) continue;

        auto &t = r.get<Transform2D>(e);
        Rectangle rect{ t.pos.x, t.pos.y, t.size.x, t.size.y };

        if (intersects(ballRect, rect)) {
            bV.vel.y *= -1;
            r.destroy(e);           // destroy block
            break;                
        }
    }
}

void Game::render() {
    // Dibujar bloques
    auto blocks = r.view<Tags::Block>();
    for (auto e : blocks) {
        if (!r.all_of<Transform2D, RenderColor>(e)) continue;
        auto &t  = r.get<Transform2D>(e);
        auto &rc = r.get<RenderColor>(e);
        if (rc.visible) DrawRectangleV(t.pos, t.size, rc.color);
    }

    // Dibujar paddle
    {
        auto &t  = r.get<Transform2D>(paddle);
        auto &rc = r.get<RenderColor>(paddle);
        if (rc.visible) DrawRectangleV(t.pos, t.size, rc.color);
    }

    // Dibujar pelota
    {
        auto &t = r.get<Transform2D>(ball);
        DrawRectangleV(t.pos, t.size, WHITE);
    }
}

void Game::frame_end() { EndDrawing(); }
void Game::clean()     { CloseWindow(); }
bool  Game::running()  { return isRunning; }

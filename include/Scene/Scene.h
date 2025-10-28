#pragma once
#include <entt/entt.hpp>
#include <raylib.h>

class Scene {
public:
    entt::registry reg;

    Texture2D bg{0};
    Texture2D hero{0};
    // Texture2D slime{0};

    void setup();
    void update();
    void render();
    void shutdown();
};

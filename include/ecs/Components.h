#pragma once
#include <raylib.h>
#include <entt/entt.hpp>

struct Transform2D {
    Vector2 pos{};
    Vector2 size{};
};

struct Velocity {
    Vector2 vel{};
};

struct AABB {
    Rectangle rect(const Transform2D& t) const {
        return Rectangle{ t.pos.x, t.pos.y, t.size.x, t.size.y };
    }
};

struct RenderColor {
    Color color{ WHITE };
    bool  visible{ true };
};

struct Health {
    int hp{1};
};

namespace Tags {
    struct Paddle {};
    struct Ball   {};
    struct Block  {};
}

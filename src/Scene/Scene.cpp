#pragma once

#include <vector>
#include <entt/entt.hpp>
// #include "ecs/Components.h"

class Entity;
class System;

class Scene{
private:
    std::vector<System*> systems;
    std::string name;

public:
    Scene(const std::string&);
    ~Scene();

    entt::registry r;

    Entity createEntity(
        const std::string& name = "NO NAME",
        int x = 0,
        int y = 0
    );

    void addSystem(System* system);

    void setup();
    void update();
    void render();
}
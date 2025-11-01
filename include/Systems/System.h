#pragma once
#ifndef SYSTEM_H_GUARD
#define SYSTEM_H_GUARD

class Scene;

class System {
protected:
    Scene* scene = nullptr;
public:
    virtual ~System() = default;
    inline void setScene(Scene* s) { scene = s; }
    virtual void setup() {}
    virtual void update() {}
    virtual void render() {}
};

#endif // SYSTEM_H_GUARD

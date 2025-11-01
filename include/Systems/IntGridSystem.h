#pragma once
#include "Systems/System.h"

class IntGridSystem : public System {
public:
    void setup() override;
    void update() override;
    void render() override;
};

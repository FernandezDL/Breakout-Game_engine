#include "System.h"
#include "Tilemap.h"

class TilemapLoaderSystem : public System {
public:
    bool done = false;
    void update() override;
};

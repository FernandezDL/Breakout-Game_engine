#pragma once
#include "System.h"
#include "Tilemap.h"

class TilemapLoaderSystem : public System {
public:
    bool done = false;
    void update() override;

    int  getCols()        const { return cols_; }
    int  getRows()        const { return rows_; }
    int  getTileWidth()   const { return tileW_; }
    int  getTileHeight()  const { return tileH_; }
    int  getPixelWidth()  const { return cols_ * tileW_; }
    int  getPixelHeight() const { return rows_ * tileH_; }

private:
    int cols_  = 0;
    int rows_  = 0;
    int tileW_ = 32;
    int tileH_ = 32;
};

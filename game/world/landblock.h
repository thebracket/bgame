#pragma once

#include <vector>
#include <string>
#include "tile.h"

using std::vector;

constexpr int landblock_height = 256;
constexpr int landblock_width  = 256;
constexpr int tiles_per_landblock = landblock_height * landblock_width;

class land_block {
public:
    land_block();
    land_block(int index_to_load);
    int idx(const int x, const int y) {
        return (y*landblock_width)+x;
    };

    int index;
    vector<tile> tiles;

    void save();

    std::string get_filename() const;
private:
    void load();
};

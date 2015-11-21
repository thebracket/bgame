#pragma once

#include <string>
#include "base_raw.h"

using std::string;

namespace raws
{

struct raw_tile : public base_raw {
    raw_tile() {
        type = TILE;
    };
    raw_tile ( const int idx ) : tile_idx ( idx ) {
        type = TILE;
    }
    int tile_idx;

    virtual int get_tile_override() override {
        return tile_idx;
    }
};

}

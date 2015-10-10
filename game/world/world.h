#pragma once

#include <array>
#include <memory>
#include "console.h"
#include "landblock.h"

using std::array;
using std::unique_ptr;

namespace world {

const int world_width = 32;
const int world_height = 32;
const int world_size = world_height * world_width;

inline int world_idx(const int x, const int y) { return (world_height * y) + x; }

// Features for use by the various other structures of the game
extern console log;
extern land_block * current_region; // Non-owning pointer to the area in which play is currently taking place

}
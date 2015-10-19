#pragma once

#include <array>
#include <memory>
#include <map>
#include "console.h"
#include "landblock.h"
#include "../../engine/virtual_terminal.h"

using std::array;
using std::unique_ptr;
using std::map;

namespace world {

const int world_width = 32;
const int world_height = 32;
const int world_size = world_height * world_width;

inline int world_idx(const int x, const int y) { return (world_height * y) + x; }

// Features for use by the various other structures of the game
extern console log;
extern land_block * current_region; // Non-owning pointer to the area in which play is currently taking place

// Convenience: storing the camera's handle globally is messy,
// but makes it quick and easy for systems that want to find it.
extern int camera_handle;
extern int cordex_handle;

// For handing data between gui_game_view and the render controller
extern map<int, engine::vterm::screen_character> entity_render_list;
}
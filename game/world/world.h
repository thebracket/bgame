#pragma once

#include <array>
#include <memory>
#include <unordered_map>
#include <fstream>
#include <string>
#include "console.h"
#include "landblock.h"
#include "../../engine/virtual_terminal.h"

using std::array;
using std::unique_ptr;
using std::unordered_map;
using std::fstream;
using std::string;

namespace world {

const int world_width = 4;
const int world_height = 4;
const int world_size = world_height * world_width;

inline int world_idx(const int x, const int y) { return (world_height * y) + x; }

// Features for use by the various other structures of the game
extern console log;
extern land_block * current_region; // Non-owning pointer to the area in which play is currently taking place

// Convenience: storing the camera's handle globally is messy,
// but makes it quick and easy for systems that want to find it.
extern int camera_handle;
extern int cordex_handle;
extern string display_day_month;
extern string display_time;
extern string display_season;
extern int stored_power;
extern int max_power;
extern float sun_angle;
extern bool paused;

// For handing data between gui_game_view and the render controller
extern unordered_map<int, int> entity_render_list;
extern unordered_map<int, bool> walk_blocked;
extern unordered_map<int, bool> view_blocked;

void load_world_constants(fstream &lbfile);
void save_world_constants(fstream &lbfile);
}
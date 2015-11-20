#pragma once

#include <array>
#include <memory>
#include <unordered_map>
#include <fstream>
#include <string>
#include <tuple>
#include <array>
#include <experimental/optional>
#include "console.h"
#include "landblock.h"
#include "../../engine/virtual_terminal.h"

using std::array;
using std::unique_ptr;
using std::unordered_map;
using std::fstream;
using std::string;
using std::tuple;
using std::experimental::optional;

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
extern bool render_graphics;

// For handing data between gui_game_view and the render controller
struct render_info_t {
  int tile_id;
  unsigned char glyph;
  color_t foreground;
  color_t background;
  bool translucent;
};

struct render_layers_t {
  optional<render_info_t> floor;
  optional<render_info_t> building;
  optional<render_info_t> top;
};

extern unordered_map<int, render_layers_t> entity_render_list;
extern unordered_map<int, bool> walk_blocked;
extern unordered_map<int, bool> view_blocked;

struct available_item {
     string name;
     std::pair<int,int> location;
};
extern unordered_map<string, vector<available_item>> inventory;

void load_world_constants(fstream &lbfile);
void save_world_constants(fstream &lbfile);
}
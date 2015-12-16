#pragma once

#include <array>
#include <memory>
#include <unordered_map>
#include <fstream>
#include <string>
#include <tuple>
#include <array>
#include "console.h"
#include "../../engine/virtual_terminal.h"
#include <boost/optional.hpp>
#include "location.hpp"

using std::array;
using std::unique_ptr;
using std::unordered_map;
using std::fstream;
using std::string;
using std::tuple;
using boost::optional;

namespace world {


// Features for use by the various other structures of the game
extern console log;

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

extern std::vector<optional<engine::vterm::screen_character>> render_list_3d;
extern std::vector<bool> walk_blocked_3d;
extern std::vector<bool> view_blocked_3d;

struct available_item {
     string name;
     location_t location;
     int entity_id;
};
extern unordered_map<string, vector<available_item>> inventory;

void load_world_constants(fstream &lbfile);
void save_world_constants(fstream &lbfile);
}

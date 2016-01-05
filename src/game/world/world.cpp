#include "world.h"
#include "../tasks/help_wanted.h"
#include "world_defs.hpp"

using std::pair;

namespace world
{

unordered_map<string, vector<available_item>> inventory;
std::vector<optional<engine::vterm::screen_character>> render_list_3d(
		REGION_TILES);

string display_day_month;
string display_time;
string display_season;
float sun_angle = 0.0F;
int max_power = 100;
bool paused = true;
bool render_graphics = true;
bool omniscience = false;

}


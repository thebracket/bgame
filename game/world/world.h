#pragma once

#include "console.h"
#include "landblock.h"

namespace world {

void make_radius_visible(const int x, const int y, const int radius);
void reset_visibility();
  
// Temporary globals to get going...
extern int player_x;
extern int player_y;

// Features for use by the various other structures of the game
extern console log;
extern land_block current_region;

}
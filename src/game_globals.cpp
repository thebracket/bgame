#include "game_globals.hpp"

planet_t planet;
region_t current_region;
std::size_t camera_entity;
world_position_t * camera_position;
int clip_left = -1;
int clip_right = -1;
int clip_top = -1;
int clip_bottom = -1;

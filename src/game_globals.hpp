#pragma once

#include "planet/planet.hpp"
#include "planet/region.hpp"
#include "components/components.hpp"

extern planet_t planet;
extern region_t current_region;
extern std::size_t camera_entity;
extern world_position_t * camera_position;
extern calendar_t * calendar;
extern int clip_left;
extern int clip_right;
extern int clip_top;
extern int clip_bottom;

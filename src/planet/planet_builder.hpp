#pragma once

#include <rltk.hpp>
#include <mutex>
#include <vector>
#include <string>
#include "region.hpp"

void build_planet();
bool is_planet_build_complete();
extern std::mutex planet_builder_lock;
extern std::vector<rltk::vchar> planet_builder_display;
extern std::string planet_builder_status;
void setup_build_planet(int width, int height);
int get_ground_z(region_t &region, const int x, const int y);
void add_construction(region_t &region, const int x, const int y, const int z, const uint16_t type);

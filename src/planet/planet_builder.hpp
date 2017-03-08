#pragma once

#include <rltk.hpp>
#include <mutex>
#include <vector>
#include <string>
#include <memory>
#include "region.hpp"
#include "planet.hpp"

void build_planet(const int &seed, const int &water_divisor, const int &plains_divisor, const int &starting_settlers,
    const bool &strict_beamdown);
bool is_planet_build_complete();
extern std::mutex planet_builder_lock;
extern std::unique_ptr<std::vector<rltk::vchar>> planet_builder_display;
extern std::string planet_builder_status;

void setup_build_planet(int width, int height);

void set_worldgen_status(const std::string &status);
void planet_display_update_altitude(planet_t &planet);
void planet_display_update_zoomed(planet_t &planet, const int world_x, const int world_y);
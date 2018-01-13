#pragma once

#include "../stdafx.h"
#include "region/region.hpp"
#include "planet.hpp"

struct worldgen_display_t {
    int texture_id = 0;
    uint8_t altitude = 0;
    bool rivers = false;
};

void build_planet(const int &seed, const int &water_divisor, const int &plains_divisor, const int &starting_settlers,
    const bool &strict_beamdown, const bool &ascii_mode);
bool is_planet_build_complete();
extern std::mutex planet_builder_lock;
extern std::unique_ptr<std::vector<worldgen_display_t>> planet_builder_display;
extern std::string planet_builder_status;

void setup_build_planet();

void set_worldgen_status(const std::string &status);
void planet_display_update_altitude(planet_t &planet);
void planet_display_update_zoomed(planet_t &planet, const int world_x, const int world_y);
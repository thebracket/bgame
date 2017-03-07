#include "starting_point.hpp"
#include <iostream>


bool has_civilization(planet_t &planet, const int &x, const int &y) {
    const int pidx = planet.idx(x,y);
    if (planet.civs.region_info[pidx].settlement_size > 0) return true;
    return false;
}

bool has_river(planet_t &planet, const int &x, const int &y) {
    for (const river_t &river : planet.rivers) {
        if (river.start_x == x && river.start_y == y) return true;
        for (const river_step_t &s : river.steps) {
            if (s.x == x && s.y == y) return true;
        }
    }
    return false;
}

bool blighted(planet_t &planet, const int &x, const int &y) {
    const int pidx = planet.idx(x,y);
    if (planet.civs.region_info[pidx].blight_level > 0) return true;
    return false;
}


std::pair<int,int> builder_select_starting_region(planet_t &planet) {
    std::pair<int,int> coords = std::make_pair(WORLD_WIDTH/2, WORLD_HEIGHT/2);

    bool ok = false;
    while (!ok) {
        ok = true;
        if (planet.landblocks[planet.idx(coords.first, coords.second)].type == block_type::WATER) ok = false;
        if (planet.biomes[planet.landblocks[planet.idx(coords.first, coords.second)].biome_idx].mean_altitude < planet.water_height+2) ok = false;
        if (!has_civilization(planet, coords.first, coords.second)) ok = false;
        //if (!has_river(planet, coords.first, coords.second)) ok = false;
        if (blighted(planet, coords.first, coords.second)) ok = false; // Force blight region for debug

        if (!ok) {
            --coords.first;
            if (coords.first < 0) {
                coords.first = WORLD_WIDTH-1;
                --coords.second;
            }
        }        
    }
    //std::cout << biome_defs[planet.biomes[planet.landblocks[planet.idx(coords.first, coords.second)].biome_idx].type].name << "\n";
    
    return coords;
}

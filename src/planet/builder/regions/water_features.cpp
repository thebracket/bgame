#include "water_features.hpp"
#include "../../planet_builder.hpp"
#include "../../region.hpp"

using namespace rltk;
using namespace region;

void add_dig_target(int X, int Y, int radius, int depth, std::unordered_map<int, region_water_feature_tile> &dig_targets,
    std::vector<uint8_t> &pooled_water, std::vector<uint8_t> &heightmap) {

    int min_altitude = REGION_DEPTH;
    for (int y=0-radius; y<radius; ++y) {
        for (int x = 0 - radius; x < radius; ++x) {
            const int actual_x = X + x;
            const int actual_y = Y + y;
            const int idx = (actual_y * REGION_WIDTH) + actual_x;
            if (heightmap[idx] < min_altitude) min_altitude = heightmap[idx];
        }
    }

    for (int y=0-radius; y<radius; ++y) {
        for (int x=0-radius; x<radius; ++x) {
            const int actual_x = X+x;
            const int actual_y = Y+y;
            if (actual_x > -1 && actual_x < REGION_WIDTH && actual_y > -1 && actual_y < REGION_HEIGHT) {
                const int idx = (actual_y * REGION_WIDTH) + actual_x;                
                if (dig_targets.find(idx) == dig_targets.end()) {
                    const bool has_water = pooled_water[idx] > 0;
                    dig_targets[idx] = region_water_feature_tile{ idx, has_water, depth, heightmap[idx] };
                }
            }
        }
    }
}


void just_add_water(planet_t &planet, std::vector<uint8_t> &pooled_water, std::vector<uint8_t> &heightmap, std::pair<biome_t, biome_type_t> &biome, random_number_generator &rng, FastNoise &noise, std::vector<std::pair<int, uint8_t>> &water_spawners) {
    set_worldgen_status("Just add water...");

    bool river_starts_here = false;
    bool river_terminates_here = false;
    bool has_river = false;

    std::array<int, 4> river_entry;
    int river_exit = 0;
    std::fill(river_entry.begin(), river_entry.end(), 0);

    for (const river_t &river : planet.rivers) {
        if (river.start_x == region_x() && river.start_y == region_y()) {
            river_starts_here = true;
            has_river = true;
        }

        int last_x = river.start_x;
        int last_y = river.start_y;

        std::size_t i=0;
        for (const river_step_t &s : river.steps) {
            if (s.x == region_x() && s.y == region_y()) {
                has_river = true;

                if (last_x < s.x) { ++river_entry[0]; }
                if (last_x > s.x) { ++river_entry[1]; }
                if (last_y < s.y) { ++river_entry[2]; }
                if (last_y > s.y) { ++river_entry[3]; }

                if (i+1 < river.steps.size()) {
                    const int next_x = river.steps[i+1].x;
                    const int next_y = river.steps[i+1].y;

                    if (next_x < s.x) { river_exit = 1; }
                    if (next_x > s.x) { river_exit = 2; }
                    if (next_y < s.y) { river_exit = 3; }
                    if (next_y > s.y) { river_exit = 4; }
                } else {
                    river_terminates_here = true;
                }
            }
            last_x = s.x;
            last_y = s.y;
            ++i;
        }
    }

    if (!has_river) return;

    // Determine a confluence point - mid-point for the rivers
    bool mid_ok = false;
    int midpoint_x, midpoint_y;
    while (!mid_ok) {
        midpoint_x = rng.roll_dice(1, REGION_WIDTH/2) + REGION_WIDTH/4;
        midpoint_y = rng.roll_dice(1, REGION_HEIGHT/2) + REGION_HEIGHT/4;
        const float d = distance2d(midpoint_x, midpoint_y, REGION_WIDTH/2, REGION_HEIGHT/2);
        if (d > 15.0f) mid_ok = true;
    }

    // Run rivers to the confluence
    std::unordered_map<int, region_water_feature_tile> dig_targets;

    auto dig_river = [&heightmap, &pooled_water, &dig_targets] (int X, int Y) {
        add_dig_target(X, Y, 2, 2, dig_targets, pooled_water, heightmap);
    };
    auto dig_exit_river = [&heightmap, &pooled_water, &dig_targets] (int X, int Y) {
        add_dig_target(X, Y, 2, 2, dig_targets, pooled_water, heightmap);
    };

    for (int i=0; i<river_entry[0]; ++i) {
        int start_x = 0;
        int start_y = rng.roll_dice(1, REGION_HEIGHT/2) + REGION_HEIGHT/4 -1;
        line_func(start_x, start_y, midpoint_x, midpoint_y, dig_river);
        water_spawners.push_back({2, (start_y * REGION_WIDTH) + start_x});
    }
    for (int i=0; i<river_entry[1]; ++i) {
        int start_x = REGION_WIDTH-1;
        int start_y = rng.roll_dice(1, REGION_HEIGHT/2) + REGION_HEIGHT/4 -1;
        line_func(start_x, start_y, midpoint_x, midpoint_y, dig_river);
        water_spawners.push_back({2, (start_y * REGION_WIDTH) + start_x});
    }
    for (int i=0; i<river_entry[2]; ++i) {
        int start_x = rng.roll_dice(1, REGION_WIDTH/2) + REGION_WIDTH/4-1;
        int start_y = 0;
        line_func(start_x, start_y, midpoint_x, midpoint_y, dig_river);
        water_spawners.push_back({2, (start_y * REGION_WIDTH) + start_x});
    }
    for (int i=0; i<river_entry[3]; ++i) {
        int start_x = rng.roll_dice(1, REGION_WIDTH/2) + REGION_WIDTH/4-1;
        int start_y = REGION_HEIGHT-1;
        line_func(start_x, start_y, midpoint_x, midpoint_y, dig_river);
        water_spawners.push_back({2, (start_y * REGION_WIDTH) + start_x});
    }

    // Run confluence to the exit
    if (!river_terminates_here) {
        int end_x, end_y;
        if (river_exit == 1) { end_x = 0; end_y = rng.roll_dice(1, REGION_HEIGHT/2) + REGION_HEIGHT/4; }
        if (river_exit == 2) { end_x = REGION_WIDTH; end_y = rng.roll_dice(1, REGION_HEIGHT/2) + REGION_HEIGHT/4; }
        if (river_exit == 3) { end_x = rng.roll_dice(1, REGION_WIDTH/2)+REGION_WIDTH/4; end_y = 0; }
        if (river_exit == 4) { end_x = rng.roll_dice(1, REGION_WIDTH/2)+REGION_WIDTH/4; end_y = REGION_HEIGHT; }
        line_func(midpoint_x, midpoint_y, end_x, end_y, dig_exit_river);
        water_spawners.push_back({3, (end_y * REGION_WIDTH) + end_x});
    }

    // Actually dig out the rivers

    // 1 - Find the lowest point on the river network
    int min_altitude = REGION_DEPTH;
    for (const auto &t : dig_targets) {
        if (t.second.altitude < min_altitude) min_altitude = t.second.altitude;
    }

    // 2 - Dig down - the rivers are (lowest-point - depth)
    for (const auto &t : dig_targets) {
        if (!t.second.has_water_already) {
            heightmap[t.first] = t.second.altitude - t.second.depth;
            pooled_water[t.first] = (t.second.depth-1)*10;
        }
    }
}

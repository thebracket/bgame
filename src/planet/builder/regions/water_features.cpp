#include "water_features.hpp"
#include "../../planet_builder.hpp"
#include "../../region/region.hpp"
#include "../../../bengine/geometry.hpp"

using namespace region;

void add_dig_target(int X, int Y, int radius, int depth, std::unordered_map<int, region_water_feature_tile> &dig_targets,
    std::vector<uint8_t> &pooled_water, std::vector<uint8_t> &heightmap) noexcept {

    auto min_altitude = REGION_DEPTH;
    for (auto y=0-radius; y<radius; ++y) {
        for (auto x = 0 - radius; x < radius; ++x) {
            const auto actual_x = X + x;
            const auto actual_y = Y + y;
            const auto idx = (actual_y * REGION_WIDTH) + actual_x;
            if (idx > 0 && heightmap[idx] < min_altitude) min_altitude = heightmap[idx];
        }
    }

    for (auto y=0-radius; y<radius; ++y) {
        for (auto x=0-radius; x<radius; ++x) {
            const auto actual_x = X+x;
            const auto actual_y = Y+y;
            if (actual_x > -1 && actual_x < REGION_WIDTH && actual_y > -1 && actual_y < REGION_HEIGHT) {
                const auto idx = (actual_y * REGION_WIDTH) + actual_x;
                if (dig_targets.find(idx) == dig_targets.end()) {
                    const auto has_water = pooled_water[idx] > 0;
                    dig_targets[idx] = region_water_feature_tile{ idx, has_water, depth, heightmap[idx] };
                }
            }
        }
    }
}


void just_add_water(planet_t &planet, std::vector<uint8_t> &pooled_water, std::vector<uint8_t> &heightmap, std::pair<biome_t, biome_type_t> &biome, bengine::random_number_generator &rng, FastNoise &noise, std::vector<std::pair<int, uint8_t>> &water_spawners) noexcept {
    set_worldgen_status("Just add water...");

	auto river_starts_here = false;
	auto river_terminates_here = false;
	auto has_river = false;

    std::array<int, 4> river_entry{};
	auto river_exit = 0;
    std::fill(river_entry.begin(), river_entry.end(), 0);

    for (const auto &river : planet.rivers) {
        if (river.start_x == region_x() && river.start_y == region_y()) {
            river_starts_here = true;
            has_river = true;
        }

		auto last_x = river.start_x;
		auto last_y = river.start_y;

        std::size_t i=0;
        for (const auto &s : river.steps) {
            if (s.x == region_x() && s.y == region_y()) {
                has_river = true;

                if (last_x < s.x) { ++river_entry[0]; }
                if (last_x > s.x) { ++river_entry[1]; }
                if (last_y < s.y) { ++river_entry[2]; }
                if (last_y > s.y) { ++river_entry[3]; }

                if (i+1 < river.steps.size()) {
                    const auto next_x = river.steps[i+1].x;
                    const auto next_y = river.steps[i+1].y;

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
	auto mid_ok = false;
    int midpoint_x, midpoint_y;
    while (!mid_ok) {
        midpoint_x = rng.roll_dice(1, REGION_WIDTH/2) + REGION_WIDTH/4;
        midpoint_y = rng.roll_dice(1, REGION_HEIGHT/2) + REGION_HEIGHT/4;
        const auto d = bengine::distance2d(midpoint_x, midpoint_y, REGION_WIDTH/2, REGION_HEIGHT/2);
        if (d > 15.0f) mid_ok = true;
    }

    // Run rivers to the confluence
    std::unordered_map<int, region_water_feature_tile> dig_targets;

    const auto dig_river = [&heightmap, &pooled_water, &dig_targets] (int X, int Y) {
        add_dig_target(X, Y, 2, 2, dig_targets, pooled_water, heightmap);
    };
    const auto dig_exit_river = [&heightmap, &pooled_water, &dig_targets] (int X, int Y) {
        add_dig_target(X, Y, 2, 2, dig_targets, pooled_water, heightmap);
    };

    for (auto i=0; i<river_entry[0]; ++i) {
		auto start_x = 0;
		auto start_y = rng.roll_dice(1, REGION_HEIGHT/2) + REGION_HEIGHT/4 -1;
        bengine::line_func(start_x, start_y, midpoint_x, midpoint_y, dig_river);
        water_spawners.push_back({1, (start_y * REGION_WIDTH) + start_x});
    }
    for (int i=0; i<river_entry[1]; ++i) {
        int start_x = REGION_WIDTH-1;
        int start_y = rng.roll_dice(1, REGION_HEIGHT/2) + REGION_HEIGHT/4 -1;
        bengine::line_func(start_x, start_y, midpoint_x, midpoint_y, dig_river);
        water_spawners.push_back({1, (start_y * REGION_WIDTH) + start_x});
    }
    for (int i=0; i<river_entry[2]; ++i) {
        int start_x = rng.roll_dice(1, REGION_WIDTH/2) + REGION_WIDTH/4-1;
        int start_y = 0;
        bengine::line_func(start_x, start_y, midpoint_x, midpoint_y, dig_river);
        water_spawners.push_back({1, (start_y * REGION_WIDTH) + start_x});
    }
    for (int i=0; i<river_entry[3]; ++i) {
        int start_x = rng.roll_dice(1, REGION_WIDTH/2) + REGION_WIDTH/4-1;
        int start_y = REGION_HEIGHT-1;
        bengine::line_func(start_x, start_y, midpoint_x, midpoint_y, dig_river);
        water_spawners.push_back({1, (start_y * REGION_WIDTH) + start_x});
    }

    // Run confluence to the exit
    if (!river_terminates_here) {
        int end_x, end_y;
        if (river_exit == 1) { end_x = 0; end_y = rng.roll_dice(1, REGION_HEIGHT/2) + REGION_HEIGHT/4; }
        if (river_exit == 2) { end_x = REGION_WIDTH; end_y = rng.roll_dice(1, REGION_HEIGHT/2) + REGION_HEIGHT/4; }
        if (river_exit == 3) { end_x = rng.roll_dice(1, REGION_WIDTH/2)+REGION_WIDTH/4; end_y = 0; }
        if (river_exit == 4) { end_x = rng.roll_dice(1, REGION_WIDTH/2)+REGION_WIDTH/4; end_y = REGION_HEIGHT; }
        bengine::line_func(midpoint_x, midpoint_y, end_x, end_y, dig_exit_river);
        water_spawners.push_back({0, (end_y * REGION_WIDTH) + end_x});
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
            heightmap[t.second.idx] = min_altitude-1;
            pooled_water[t.second.idx] = 20;
        }
    }
}

#include "planet_builder.hpp"
#include "../raws/raws.hpp"
#include "builder/settler_builder.hpp"
#include "builder/map_builder.hpp"
#include "builder/biome_builder.hpp"
#include "builder/region_builder.hpp"
#include "builder/history_builder.hpp"
#include "../FastNoise/FastNoise.h"

#include <atomic>
#include <iostream>
#include <limits>
#include <thread>
#include <rltk.hpp>
#include <sstream>

using namespace rltk;
using namespace rltk::colors;

std::atomic<bool> planet_build_done;
std::mutex planet_builder_lock;
int planet_build_term_width;
int planet_build_term_height;
std::unique_ptr<std::vector<rltk::vchar>> planet_builder_display;
std::string planet_builder_status = "";

bool is_planet_build_complete() {
	return planet_build_done.load();
}

void setup_build_planet(int width, int height) {
	planet_build_term_width = width;
	planet_build_term_height = height;
	planet_builder_display = std::make_unique<std::vector<rltk::vchar>>();
	planet_builder_display->resize(width*height);
	std::fill(planet_builder_display->begin(), planet_builder_display->end(), vchar{0, WHITE, BLACK});
}

inline void set_planet_display_char(const int &block_idx, const int &idx, planet_t &planet) {
	double hotness = ((double)planet.landblocks[block_idx].temperature_c + 88.0) / 146.0;
	const color_t temperature = lerp(BLUE, RED, hotness);
	uint8_t col = planet.landblocks[block_idx].height;
	const uint8_t zero = 0;

	rltk::color_t bg = BLACK;
	const int biome_idx = planet.landblocks[block_idx].biome_idx;
	if (biome_idx > -1) {
		//bg = biome_defs[planet.biomes[biome_idx].type].worldgen_color;
		bg = rltk::colors::BLACK;
	}

	if (planet.landblocks[block_idx].type == block_type::NONE) {
		(*planet_builder_display.get())[idx] = rltk::vchar{178, color_t{col, col, col}, temperature};
	} else if (planet.landblocks[block_idx].type == block_type::WATER) {
		(*planet_builder_display.get())[idx] = rltk::vchar{178, color_t{zero, zero, col}, bg};
	} else if (planet.landblocks[block_idx].type == block_type::PLAINS) {
		(*planet_builder_display.get())[idx] = rltk::vchar{178, color_t{zero, col, zero}, bg};
	} else if (planet.landblocks[block_idx].type == block_type::HILLS) {
		(*planet_builder_display.get())[idx] = rltk::vchar{30, color_t{zero, col, zero}, bg};
	} else if (planet.landblocks[block_idx].type == block_type::MARSH) {
		(*planet_builder_display.get())[idx] = rltk::vchar{'~', color_t{zero, col, zero}, bg};
	} else if (planet.landblocks[block_idx].type == block_type::PLATEAU) {
		(*planet_builder_display.get())[idx] = rltk::vchar{239, color_t{col, col, col}, bg};
	} else if (planet.landblocks[block_idx].type == block_type::HIGHLANDS) {
		(*planet_builder_display.get())[idx] = rltk::vchar{178, color_t{col, col, col}, bg};
	} else if (planet.landblocks[block_idx].type == block_type::COASTAL) {
		(*planet_builder_display.get())[idx] = rltk::vchar{176, color_t{zero, col, zero}, bg};
	} else if (planet.landblocks[block_idx].type == block_type::SALT_MARSH) {
		(*planet_builder_display.get())[idx] = rltk::vchar{247, color_t{zero, col, zero}, bg};
	} else {
		(*planet_builder_display.get())[idx] = rltk::vchar{30, color_t{col, col, col}, bg};
	}

	for (const river_t &r : planet.rivers) {
		if (planet.idx(r.start_x, r.start_y) == block_idx) {
			(*planet_builder_display.get())[idx] = rltk::vchar{'%', color_t{0, 0, 255}, bg};
		}
		for (const river_step_t &s : r.steps) {
			if (planet.idx(s.x, s.y) == block_idx) {
				(*planet_builder_display.get())[idx] = rltk::vchar{'%', color_t{0, 0, 255}, bg};
			}
		}
	}
}

void planet_display_update_zoomed(planet_t &planet, const int world_x, const int world_y) {
	const int half_term_x = planet_build_term_width / 2;
	const int half_term_y = planet_build_term_height / 2;

	int left_x = world_x - half_term_x;
	int right_x = world_x + half_term_x;
	int top_y = world_y - half_term_y;
	int bottom_y = world_y + half_term_y;

	if (left_x < 0) {
		left_x = 0;
		right_x = planet_build_term_width;
	}
	if (right_x > WORLD_WIDTH) {
		left_x = WORLD_WIDTH - planet_build_term_width;
		right_x = WORLD_WIDTH;
	}
	if (top_y < 0) {
		top_y = 0;
		bottom_y = planet_build_term_height;
	}
	if (bottom_y > WORLD_HEIGHT) {
		bottom_y = WORLD_HEIGHT;
		top_y = WORLD_HEIGHT - planet_build_term_height;
	}

	int Y = 0;
	for (int y=top_y; y<bottom_y; ++y) {
		int X = 0;
		for (int x=left_x; x<right_x; ++x) {
			const int idx = (planet_build_term_width * Y) + X;
			const int block_idx = planet.idx(x, y);			
			if (block_idx < planet.landblocks.size() && idx < planet_build_term_height*planet_build_term_width)
			{
				set_planet_display_char(block_idx, idx, planet);
				for (const auto &s : planet.civs.settlements) {
					if (s.world_x == x && s.world_y == y) {
						color_t civ_color = color_t{planet.civs.civs[s.civ_id].r, planet.civs.civs[s.civ_id].g, planet.civs.civs[s.civ_id].b};
						if (s.status == 0) (*planet_builder_display.get())[idx] = rltk::vchar{9, rltk::colors::DARK_GREY, rltk::colors::BLACK};
						if (s.status == 1) (*planet_builder_display.get())[idx] = rltk::vchar{'*', rltk::colors::WHITE, civ_color};
						if (s.status == 2) (*planet_builder_display.get())[idx] = rltk::vchar{9, rltk::colors::WHITE, civ_color};
					}
				}
			}
			++X;
		}
		++Y;
	}
}

void planet_display_update_altitude(planet_t &planet) {
	const int stride_x = WORLD_WIDTH / planet_build_term_width;
	const int stride_y = WORLD_HEIGHT / planet_build_term_height;

	planet_builder_lock.lock();
	for (int y=0; y<planet_build_term_height; ++y) {
		for (int x=0; x<planet_build_term_width; ++x) {
			const int idx = (planet_build_term_width * y) + x;
			const int block_idx = planet.idx(x*stride_x, y*stride_y);			
			set_planet_display_char(block_idx, idx, planet);
		}
	}
	planet_builder_lock.unlock();
}

void set_worldgen_status(const std::string &status) {
	planet_builder_lock.lock();
	planet_builder_status = status;
	planet_builder_lock.unlock();
}

void builder_save_planet(planet_t &planet) {
	planet_builder_lock.lock();
	planet_builder_status = "Saving the world. To disk, sadly.";
	planet_builder_lock.unlock();
	save_planet(planet);
	//std::this_thread::sleep_for(std::chrono::seconds(10));
}

void build_planet() {
	planet_build_done.store(false);

	random_number_generator rng;
	planet_t planet;
	planet.rng_seed = rng.initial_seed;
	const int perlin_seed = rng.roll_dice(1, std::numeric_limits<int>::max());
	planet.perlin_seed = perlin_seed;
	std::cout << "Planet seed: " << planet.perlin_seed << "\n";

	// Make a zero-height map
	planet_zero_fill(planet);

	// Noise-based world map
	FastNoise noise = planet_noise_map(planet, perlin_seed);

	// Divide types by height
	planet_base_type_allocation(planet);

	// Find coastlines
	planet_mark_coastlines(planet);

	// Adjust rainfall
	planet_rainfall(planet);

	// Make a biome map
	build_biomes(planet, rng);

	// Run rivers
	planet_rivers(planet, rng);

	// Run history
	planet_build_initial_civs(planet, rng);
	planet_build_initial_history(planet, rng);

	// Save it to disk
	builder_save_planet(planet);

	// Select a crash site
	auto crash_site = builder_select_starting_region(planet);
	planet_display_update_zoomed(planet, crash_site.first, crash_site.second);

	// Materialize this region
	build_region(planet, crash_site, rng, noise);
	builder_save_planet(planet);

	planet_build_done.store(true);
}
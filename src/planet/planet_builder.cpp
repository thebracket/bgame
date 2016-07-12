#include "planet_builder.hpp"
#include "planet.hpp"
#include "../components/components.hpp"
#include "../raws/raws.hpp"
#include "builder/settler_builder.hpp"
#include "builder/build_utils.hpp"

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

void planet_display_update_altitude(planet_t &planet) {
	const int stride_x = WORLD_WIDTH / planet_build_term_width;
	const int stride_y = WORLD_HEIGHT / planet_build_term_height;

	planet_builder_lock.lock();
	for (int y=0; y<planet_build_term_height; ++y) {
		for (int x=0; x<planet_build_term_width; ++x) {
			const int idx = (planet_build_term_width * y) + x;
			const int block_idx = planet.idx(x*stride_x, y*stride_y);

			uint8_t col = planet.landblocks[block_idx].height;
			const uint8_t zero = 0;
			if (planet.landblocks[block_idx].type == block_type::NONE) {
				(*planet_builder_display.get())[idx] = rltk::vchar{219, color_t{col, col, col}, BLACK};
			} else if (planet.landblocks[block_idx].type == block_type::WATER) {
				(*planet_builder_display.get())[idx] = rltk::vchar{219, color_t{zero, zero, col}, BLACK};
			} else if (planet.landblocks[block_idx].type == block_type::PLAINS) {
				(*planet_builder_display.get())[idx] = rltk::vchar{219, color_t{zero, col, zero}, BLACK};
			} else if (planet.landblocks[block_idx].type == block_type::HILLS) {
				(*planet_builder_display.get())[idx] = rltk::vchar{30, color_t{zero, col, zero}, BLACK};
			} else if (planet.landblocks[block_idx].type == block_type::MARSH) {
				(*planet_builder_display.get())[idx] = rltk::vchar{'~', color_t{zero, col, zero}, BLACK};
			} else if (planet.landblocks[block_idx].type == block_type::PLATEAU) {
				(*planet_builder_display.get())[idx] = rltk::vchar{239, color_t{col, col, col}, BLACK};
			} else if (planet.landblocks[block_idx].type == block_type::HIGHLANDS) {
				(*planet_builder_display.get())[idx] = rltk::vchar{219, color_t{col, col, col}, BLACK};
			} else if (planet.landblocks[block_idx].type == block_type::COASTAL) {
				(*planet_builder_display.get())[idx] = rltk::vchar{176, color_t{zero, col, zero}, BLACK};
			} else {
				(*planet_builder_display.get())[idx] = rltk::vchar{30, color_t{col, col, col}, BLACK};
			}
		}
	}
	planet_builder_lock.unlock();
}

void set_worldgen_status(const std::string &status) {
	planet_builder_lock.lock();
	planet_builder_status = status;
	planet_builder_lock.unlock();
}

void planet_zero_fill(planet_t &planet) {
	set_worldgen_status("Building initial ball of mud");

	planet.landblocks.resize(WORLD_HEIGHT * WORLD_WIDTH);
	std::fill(planet.landblocks.begin(), planet.landblocks.end(), block_t{0,0});
}

inline uint8_t noise_to_planet_height(const double &n) {
	return n * 200.0;
}

constexpr double NOISE_SIZE = 2.0;

inline double noise_x(const int world_x, const int region_x) {
	const double big_x = (world_x * WORLD_WIDTH) + region_x;
	return (big_x / ((double)WORLD_WIDTH * (double)REGION_WIDTH)) * NOISE_SIZE;
}

inline double noise_y(const int world_y, const int region_y) {
	const double big_y = (world_y * WORLD_HEIGHT) + region_y;
	return (big_y / ((double)WORLD_HEIGHT * (double)REGION_HEIGHT)) * NOISE_SIZE;
}

perlin_noise planet_noise_map(planet_t &planet, const int &perlin_seed) {
	constexpr int REGION_FRACTION_TO_CONSIDER = 32;
	constexpr int octaves = 7;
	constexpr double persistence = 0.5;
	constexpr double frequency = 3.0;
	
	set_worldgen_status("Dividing the heavens from the earth: starting");
	perlin_noise noise(perlin_seed);

	// Determine height of the region block as an average of the containing tiles
	for (int y=0; y<WORLD_HEIGHT; ++y) {
		for (int x=0; x<WORLD_WIDTH; ++x) {
			long total_height = 0L;

			uint8_t max = 0.0;
			uint8_t min = 256.0;
			int n_tiles = 0;
			for (int y1=0; y1<REGION_HEIGHT/REGION_FRACTION_TO_CONSIDER; ++y1) {
				for (int x1=0; x1<REGION_WIDTH/REGION_FRACTION_TO_CONSIDER; ++x1) {
					const double nh = noise.noise_octaves ( noise_x(x,x1*REGION_FRACTION_TO_CONSIDER), noise_y(y,y1*REGION_FRACTION_TO_CONSIDER), 0.0, octaves, persistence, frequency );
					const uint8_t n = noise_to_planet_height(nh);
					if (n < min) min = n;
					if (n > max) max = n;
					total_height += n;
					++n_tiles;
				}
			}
			planet.landblocks[planet.idx(x,y)].height = total_height / n_tiles;
			planet.landblocks[planet.idx(x,y)].type = 0;
			planet.landblocks[planet.idx(x,y)].variance = max - min;
		}
		std::stringstream ss;
		double percent = (double)y / (double)WORLD_HEIGHT;
		ss << "Dividing heavens from the earth: " << (percent*100) << "%";
		set_worldgen_status(ss.str());
		planet_display_update_altitude(planet);
	}

	return noise;
}

int planet_determine_proportion(planet_t &planet, int &candidate, int target) {
	int count = 0;
	while (count < target) {
		const int count = std::count_if(planet.landblocks.begin(), planet.landblocks.end(), [candidate] (const block_t &block) {
			return block.height <= candidate;
		});
		if (count >= target) {
			return candidate;
		} else {
			++candidate;
		}
	}
	throw std::runtime_error("Messed up landblocks!");
}

void planet_base_type_allocation(planet_t &planet) {
	set_worldgen_status("Dividing the waters from the earth");

	int candidate = 0;
	const int n_cells = WORLD_HEIGHT * WORLD_WIDTH;
	const int n_cells_water = (n_cells / 3);
	const int n_cells_plains = (n_cells / 3) + n_cells_water;
	const int n_cells_hills = (n_cells / 6) + n_cells_plains;
	
	planet.water_height = planet_determine_proportion(planet, candidate, n_cells_water);
	planet.plains_height = planet_determine_proportion(planet, candidate, n_cells_plains);
	planet.hills_height = planet_determine_proportion(planet, candidate, n_cells_hills);

	std::cout << "Heights (water/plains/hills): " << +planet.water_height << " / " << +planet.plains_height << " / " << +planet.hills_height << "\n";

	for (block_t &block : planet.landblocks) {
		if (block.height <= planet.water_height) {
			block.type = block_type::WATER;
			if (block.height + block.variance/2 > planet.water_height) block.type = block_type::MARSH;
		} else if (block.height <= planet.plains_height) {
			block.type = block_type::PLAINS;
			if (block.height - block.variance/2 > planet.water_height) block.type = block_type::MARSH;
		} else if (block.height <= planet.hills_height) {
			block.type = block_type::HILLS;
			if (block.variance < 3) block.type = block_type::HIGHLANDS;
		} else {
			block.type = block_type::MOUNTAINS;
			if (block.variance < 2) block.type = block_type::PLATEAU;
		}
		planet_display_update_altitude(planet);
	}
}

void planet_mark_coastlines(planet_t &planet) {
	set_worldgen_status("Crinkling the coastlines");
	for (int y=1; y<WORLD_HEIGHT-1; ++y) {
		for (int x=1; x<WORLD_WIDTH-1; ++x) {
			if (planet.landblocks[planet.idx(x,y)].type > block_type::WATER) {
				if (
					planet.landblocks[planet.idx(x-1,y-1)].type == block_type::WATER ||
					planet.landblocks[planet.idx(x,y-1)].type == block_type::WATER ||
					planet.landblocks[planet.idx(x+1,y-1)].type == block_type::WATER ||
					planet.landblocks[planet.idx(x-1,y)].type == block_type::WATER ||
					planet.landblocks[planet.idx(x+1,y)].type == block_type::WATER ||
					planet.landblocks[planet.idx(x-1,y+1)].type == block_type::WATER ||
					planet.landblocks[planet.idx(x,y+1)].type == block_type::WATER ||
					planet.landblocks[planet.idx(x+1,y+1)].type == block_type::WATER
				) {
					planet.landblocks[planet.idx(x,y)].type = block_type::COASTAL;
				}
			}
		}
		planet_display_update_altitude(planet);
	}
}

void builder_save_planet(planet_t &planet) {
	planet_builder_lock.lock();
	planet_builder_status = "Saving the world. To disk, sadly.";
	planet_builder_lock.unlock();
	save_planet(planet);
	std::this_thread::sleep_for(std::chrono::seconds(10));
}

void build_planet() {
	planet_build_done.store(false);

	random_number_generator rng;
	planet_t planet;
	planet.rng_seed = rng.initial_seed;
	const int perlin_seed = rng.roll_dice(1, std::numeric_limits<int>::max());
	planet.perlin_seed = perlin_seed;

	// Make a zero-height map
	planet_zero_fill(planet);

	// Noise-based world map
	perlin_noise noise = planet_noise_map(planet, perlin_seed);

	// Divide types by height
	planet_base_type_allocation(planet);

	// Find coastlines
	planet_mark_coastlines(planet);

	// Make a biome map
	//build_biomes(planet, rng);

	// Save it to disk
	builder_save_planet(planet);

	/*
	// Select a crash site
	auto crash_site = builder_select_starting_region(planet, min, max);

	// Materialize this region
	build_region(planet, crash_site, rng);

	*/
	planet_build_done.store(true);
}
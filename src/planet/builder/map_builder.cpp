#include "map_builder.hpp"
#include "../planet_builder.hpp"
#include "noise_helper.hpp"
#include <sstream>
#include <fmt/format.h>

void planet_zero_fill(planet_t &planet) noexcept {
	set_worldgen_status("Building initial ball of mud");

	planet.landblocks.resize(WORLD_HEIGHT * WORLD_WIDTH);
	std::fill(planet.landblocks.begin(), planet.landblocks.end(), block_t{});
	planet.migrant_counter = 0;
	planet.remaining_settlers = 200;
	planet.civs.region_info.resize(WORLD_TILES_COUNT);
}

FastNoise planet_noise_map(planet_t &planet, const int &perlin_seed) noexcept {
	
	set_worldgen_status("Dividing the heavens from the earth: starting");
	FastNoise noise(perlin_seed);
	noise.SetNoiseType(FastNoise::GradientFractal);
	noise.SetFractalType(FastNoise::FBM);
	noise.SetFractalOctaves(octaves);
	noise.SetFractalGain(persistence);
	noise.SetFractalLacunarity(frequency);

	constexpr auto max_temperature = 56.7F;
	constexpr auto min_temperature = -55.2F;
	constexpr auto temperature_range = max_temperature - min_temperature;
	constexpr auto half_planet_height = WORLD_HEIGHT / 2.0F;

	// Determine height of the region block as an average of the containing tiles
	#pragma omp parallel for
	for (auto y=0; y<WORLD_HEIGHT; ++y) {
		const auto distance_from_equator = std::abs((WORLD_HEIGHT/2)-y);
		const auto temp_range_pct = 1.0F - (static_cast<float>(distance_from_equator) / half_planet_height);
		const auto base_temp_by_latitude = ((temp_range_pct * temperature_range) + min_temperature);
		//std::cout << y << "/" << distance_from_equator << "/" << temp_range_pct << "/" << base_temp_by_latitude << "\n";			
		for (auto x=0; x<WORLD_WIDTH; ++x) {
			auto total_height = 0L;

			uint8_t max = 0;
			auto min = std::numeric_limits<uint8_t>::max();
			auto n_tiles = 0;
			for (auto y1=0; y1<REGION_HEIGHT/REGION_FRACTION_TO_CONSIDER; ++y1) {
				for (auto x1=0; x1<REGION_WIDTH/REGION_FRACTION_TO_CONSIDER; ++x1) {
					const auto nh = noise.GetNoise ( noise_x(x,x1*REGION_FRACTION_TO_CONSIDER), noise_y(y,y1*REGION_FRACTION_TO_CONSIDER) );
					//std::cout << nh << "\n";
					const auto n = noise_to_planet_height(nh);
					if (n < min) min = n;
					if (n > max) max = n;
					total_height += n;
					++n_tiles;
				}
			}
			planet.landblocks[planet.idx(x,y)].height = static_cast<uint8_t>(total_height / n_tiles);
			planet.landblocks[planet.idx(x,y)].type = 0;
			planet.landblocks[planet.idx(x,y)].variance = max - min;
			const auto altitude_deduction = std::abs(planet.landblocks[planet.idx(x,y)].height-planet.water_height) / 10.0F;
			planet.landblocks[planet.idx(x,y)].temperature_c = static_cast<int8_t>(base_temp_by_latitude - altitude_deduction);
            if (planet.landblocks[planet.idx(x,y)].temperature_c < -55) planet.landblocks[planet.idx(x,y)].temperature_c = -55;
            if (planet.landblocks[planet.idx(x,y)].temperature_c > 55) planet.landblocks[planet.idx(x,y)].temperature_c = 55;

		}
		fmt::MemoryWriter ss;
		const auto percent = static_cast<double>(y) / static_cast<double>(WORLD_HEIGHT);
		ss << "Dividing heavens from the earth: " << (percent*100) << "%";
		set_worldgen_status(ss.str());
		planet_display_update_altitude(planet);
	}

	return noise;
}

int planet_determine_proportion(planet_t &planet, int &candidate, int target) noexcept {
	auto count = 0;
	while (count < target) {
		count = std::count_if(planet.landblocks.begin(), planet.landblocks.end(), [candidate] (const block_t &block) {
			return block.height <= candidate;
		});
		if (count >= target) {
			return candidate;
		} else {
			++candidate;
		}
	}
	//throw std::runtime_error("Messed up landblocks!");
}

void planet_base_type_allocation(planet_t &planet) noexcept {
	set_worldgen_status("Dividing the waters from the earth");

	int candidate = 0;
	const int remaining_divisor = 10 - (planet.water_divisor + planet.plains_divisor);
	const int n_cells = WORLD_HEIGHT * WORLD_WIDTH;
	const int n_cells_water = (n_cells / planet.water_divisor);
	const int n_cells_plains = (n_cells / planet.plains_divisor) + n_cells_water;
	const int n_cells_hills = (n_cells / remaining_divisor) + n_cells_plains;
	
	planet.water_height = planet_determine_proportion(planet, candidate, n_cells_water);
	planet.plains_height = planet_determine_proportion(planet, candidate, n_cells_plains);
	planet.hills_height = planet_determine_proportion(planet, candidate, n_cells_hills);

	//std::cout << "Heights (water/plains/hills): " << +planet.water_height << " / " << +planet.plains_height << " / " << +planet.hills_height << "\n";

	#pragma omp parallel for
	for (auto i=0; i<planet.landblocks.size(); ++i) {
		auto &block = planet.landblocks.at(i);
		if (block.height <= planet.water_height) {
			block.type = block_type::WATER;
			block.rainfall = 10;
            if (block.height + block.variance/2 > planet.water_height) block.type = block_type::SALT_MARSH;
		} else if (block.height <= planet.plains_height) {
			block.type = block_type::PLAINS;
			block.rainfall = 10;
			if (block.height - block.variance/2 > planet.water_height) block.type = block_type::MARSH;
		} else if (block.height <= planet.hills_height) {
			block.type = block_type::HILLS;
			block.rainfall = 20;
			if (block.variance < 2) {
				block.type = block_type::HIGHLANDS;
				block.rainfall = 10;
			}
		} else {
			block.type = block_type::MOUNTAINS;
			block.rainfall = 30;
			if (block.variance < 3) {
				block.type = block_type::PLATEAU;
				block.rainfall = 10;
			}
		}
		planet_display_update_altitude(planet);
	}
}

void planet_mark_coastlines(planet_t &planet) noexcept {
	set_worldgen_status("Crinkling the coastlines");
	#pragma omp parallel for
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
					planet.landblocks[planet.idx(x,y)].rainfall = 20;
				}
			}
		}
		planet_display_update_altitude(planet);
	}
}

void planet_rainfall(planet_t &planet) noexcept {
	set_worldgen_status("Adjusting rain map");
	#pragma omp parallel for
	for (int y=0; y<WORLD_HEIGHT; ++y) {

		int rain_amount = 10;
		for (int x=0; x<WORLD_WIDTH; ++x) {
			if (planet.landblocks[planet.idx(x,y)].type == block_type::MOUNTAINS) {
				rain_amount -= 20;
			} else if (planet.landblocks[planet.idx(x,y)].type == block_type::HILLS) {
				rain_amount -= 10;
			} else if (planet.landblocks[planet.idx(x,y)].type == block_type::COASTAL) {
				rain_amount -= 5;
			} else {
				rain_amount += 1;
			}
			if (rain_amount < 0) rain_amount = 0;
			if (rain_amount > 20) rain_amount = 20;

			planet.landblocks[planet.idx(x,y)].rainfall += rain_amount;
			if (planet.landblocks[planet.idx(x,y)].rainfall < 0) planet.landblocks[planet.idx(x,y)].rainfall = 0;
			if (planet.landblocks[planet.idx(x,y)].rainfall > 100) planet.landblocks[planet.idx(x,y)].rainfall = 100;
		}

	}
}

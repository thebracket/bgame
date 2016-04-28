#include "planet_builder.hpp"
#include "planet.hpp"
#include "region.hpp"
#include "../components/components.hpp"

#include <atomic>
#include <iostream>
#include <limits>
#include <thread>
#include <rltk.hpp>
#include <Poco/InflatingStream.h>
#include <Poco/DeflatingStream.h>

using namespace rltk;
using namespace rltk::colors;

std::atomic<bool> planet_build_done;
std::mutex planet_builder_lock;
int planet_build_term_width;
int planet_build_term_height;
std::vector<rltk::vchar> planet_builder_display;
std::string planet_builder_status = "";

bool is_planet_build_complete() {
	return planet_build_done.load();
}

void setup_build_planet(int width, int height) {
	planet_build_term_width = width;
	planet_build_term_height = height;
	planet_builder_display.resize(width*height);
	std::fill(planet_builder_display.begin(), planet_builder_display.end(), vchar{0, WHITE, BLACK});
}

void planet_display_update_initial(planet_t &planet) {
	const int stride_x = WORLD_WIDTH / planet_build_term_width;
	const int stride_y = WORLD_HEIGHT / planet_build_term_height;

	planet_builder_lock.lock();
	for (int y=0; y<planet_build_term_height; ++y) {
		for (int x=0; x<planet_build_term_width; ++x) {
			const int idx = (planet_build_term_width * y) + x;
			const int block_idx = planet.idx(x*stride_x, y*stride_y);
			switch (planet.landblocks[block_idx].type) {
				case WATER : planet_builder_display[idx] = vchar{ 176, BLUE, BLACK}; break;
				case PLAINS : planet_builder_display[idx] = vchar{ 176, GREEN, BLACK}; break;
				case HILLS : planet_builder_display[idx] = vchar{ 30, GREEN, BLACK}; break;
				case MOUNTAINS : planet_builder_display[idx] = vchar{ 30, GREY, BLACK}; break;
			}

			if (planet.landblocks[block_idx].biome_idx != -1) {
				// TODO: Change tile
				switch (planet.biomes[planet.landblocks[block_idx].biome_idx].biome_type) {
					case GLACIER : planet_builder_display[idx] = vchar{ 219, WHITE, BLACK }; break;
					case ICY_SEA : planet_builder_display[idx].foreground = CYAN; break;
					case DUST_SEA : planet_builder_display[idx] = vchar{ 247, ORANGE, BLACK }; break;
					case TUNDRA : planet_builder_display[idx] = vchar{ 178, WHITE, DARK_GREEN }; break;
					case SWAMP : planet_builder_display[idx] = vchar{ '~', GREEN, DARK_GREEN }; break;
					case FLATLAND : planet_builder_display[idx] = vchar{ '#', GREEN, BLACK }; break;
					case WOODS : planet_builder_display[idx] = vchar{ 5, GREEN, BLACK }; break;
					case FOREST : planet_builder_display[idx] = vchar{ 5, DARK_GREEN, BLACK }; break;
					case JUNGLE : planet_builder_display[idx] = vchar{ 6, LIGHT_GREEN, BLACK }; break;
					case DESERT : planet_builder_display[idx] = vchar{ '~', ORANGE, BLACK }; break;
					case BADLANDS : planet_builder_display[idx] = vchar{ '#', DARK_RED, BLACK }; break;
					case HIGH_TUNDRA : planet_builder_display[idx] = vchar{ 176, WHITE, BLACK }; break;
					case HILL_BOG : planet_builder_display[idx] = vchar{ 176, CYAN, BLACK }; break;
					case HIGH_DESERT : planet_builder_display[idx] = vchar{ 176, ORANGE, BLACK }; break;
					case HIGH_BADLANDS : planet_builder_display[idx] = vchar{ 176, RED, BLACK }; break;
					case NORMAL_MOUNTAINS : planet_builder_display[idx] = vchar{ 30, GREY, BLACK}; break;
				}
			}
		}
	}
	planet_builder_lock.unlock();
}

void planet_display_update_regional(planet_t &planet, const int site_x, const int site_y, const int min, const int max) {
	planet_builder_lock.lock();

	const int top_y = site_y - planet_build_term_height/2;
	const int bottom_y = site_y + planet_build_term_height/2;
	const int left_x = site_x - planet_build_term_width/2;
	const int right_x = site_x + planet_build_term_width/2;

	int Y=0,X=0;
	for (int y=top_y; y<bottom_y; ++y) {
		X=0;
		for (int x=left_x; x<right_x; ++x) {
			const int idx = (planet_build_term_width * Y) + X;
			const int block_idx = planet.idx(x,y);

			switch (planet.landblocks[block_idx].type) {
				case WATER : planet_builder_display[idx] = vchar{ 176, BLUE, BLACK}; break;
				case PLAINS : {
					const int height = planet.landblocks[block_idx].height;
					const double shade_d = static_cast<double>(height+min) / static_cast<double>(max);
					const int shade = static_cast<uint8_t>(shade_d * 256.0)+127;
					color_t shading = color_t{0, shade, 0};
					planet_builder_display[idx] = vchar{ 176, shading, BLACK}; 
				} break;
				case HILLS : planet_builder_display[idx] = vchar{ 30, GREEN, BLACK}; break;
				case MOUNTAINS : planet_builder_display[idx] = vchar{ 30, GREY, BLACK}; break;
			}

			++X;
		}
		++Y;
	}
	planet_builder_lock.unlock();
}

void planet_zero_fill(planet_t &planet) {
	planet_builder_lock.lock();
	planet_builder_status = "Flatting the world";
	planet_builder_lock.unlock();

	planet.landblocks.resize(WORLD_HEIGHT * WORLD_WIDTH);
	std::fill(planet.landblocks.begin(), planet.landblocks.end(), block_t{0, WATER, -1});
}

void planet_noise_map(planet_t &planet, const int &perlin_seed) {
	planet_builder_lock.lock();
	planet_builder_status = "Building basic features";
	planet_builder_lock.unlock();

	perlin_noise mountains(perlin_seed);
	perlin_noise flatlands(std::numeric_limits<int>::max()-perlin_seed);
	perlin_noise mixer(std::numeric_limits<int>::max()+perlin_seed);
	for (int y=0; y<WORLD_HEIGHT; ++y) {
		const double Y = (double)y / WORLD_PERLIN_DIVISOR;

		for (int x=0; x<WORLD_WIDTH; ++x) {
			const double X = (double)x / WORLD_PERLIN_DIVISOR;
			const double mtn_noise_pixel = mountains.noise(X*WORLD_MOUNTAIN_MULTIPLIER, Y*WORLD_MOUNTAIN_MULTIPLIER, 0.8) * 300.0;
			const double flat_noise_pixel = flatlands.noise(X, Y, 0.8) * 10.0;
			const double mtn_pct = mixer.noise(X/2.0,Y/2.0,0.8);
			const double flat_pct = 1.0 - mtn_pct;

			const double height = (mtn_noise_pixel * mtn_pct) + (flat_noise_pixel * flat_pct);
			planet.landblocks[planet.idx(x,y)].height = height;
		}
	}
}

int plenet_determine_proportion(planet_t &planet, int &candidate, int target) {
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
	planet_builder_lock.lock();
	planet_builder_status = "Adding water, earth and making mud";
	planet_builder_lock.unlock();

	int candidate = 0;
	const int n_cells = WORLD_HEIGHT * WORLD_WIDTH;
	const int n_cells_water = n_cells / 3;
	const int n_cells_plains = (n_cells / 3) + n_cells_water;
	const int n_cells_hills = (n_cells / 6) + n_cells_plains;
	
	planet.water_height = plenet_determine_proportion(planet, candidate, n_cells_water);
	planet.plains_height = plenet_determine_proportion(planet, candidate, n_cells_plains);
	planet.hills_height = plenet_determine_proportion(planet, candidate, n_cells_hills);

	for (block_t &block : planet.landblocks) {
		if (block.height <= planet.water_height) {
			block.type = WATER;
		} else if (block.height <= planet.plains_height) {
			block.type = PLAINS;
		} else if (block.height <= planet.hills_height) {
			block.type = HILLS;
		} else {
			block.type = MOUNTAINS;
		}
	}
	planet_display_update_initial(planet);
}

std::string name_biome(const biome_t & biome) {
	std::string result = "The ";

	switch (biome.climate) {
		case ARCTIC : { result += "Frigid "; } break;
		case SUBARCTIC : { result += "Icy "; } break;
		case COLD : { result += "Cold "; } break;
		case TEMPERATE : { result += "Pleasant "; } break;
		case WARM : { result += "Warm "; } break;
		case HOT : { result += "Hot "; } break;
		case SCORCHING : { result += "Burning "; } break;
	}

	switch (biome.biome_type) {
		case GLACIER : { result += "Glacier"; } break;
		case ICY_SEA : { result += "Icy Sea"; } break;
		case DUST_SEA : { result += "Dust Sea"; } break;
		case OCEAN : { result += "Ocean"; } break;
		case TUNDRA : { result += "Tundra"; } break;
		case SWAMP : { result += "Swamp"; } break;
		case FLATLAND : { result += "Plains"; } break;
		case WOODS : { result += "Woods"; } break;
		case FOREST : { result += "Forest"; } break;
		case JUNGLE : { result += "Jungle"; } break;
		case DESERT : { result += "Desert"; } break;
		case BADLANDS : { result += "Badlands"; } break;
		case HIGH_TUNDRA : { result += "Frozen Peaks"; } break;
		case HILL_BOG : { result += "Bog"; } break;
		case HIGH_DESERT : { result += "High Desert"; } break;
		case HIGH_BADLANDS : { result += "Badland Mountains"; } break;
		case NORMAL_MOUNTAINS : { result += "Mountains"; } break;
	}

	// TODO: Add a descriptive word

	return result;
}

void build_biomes(planet_t &planet, random_number_generator &rng) {
	planet_builder_lock.lock();
	planet_builder_status = "Placing Biomes";
	planet_builder_lock.unlock();
	const int n_biomes = WORLD_HEIGHT * WORLD_WIDTH / (64 + rng.roll_dice(1,64));

	// Randomly place biome centers
	std::vector<std::pair<int,int>> centroids;
	for (int i=0; i<n_biomes; ++i) {
		centroids.push_back(std::make_pair(rng.roll_dice(1, WORLD_WIDTH), rng.roll_dice(1, WORLD_HEIGHT)));
		planet.biomes.push_back(biome_t{UNASSIGNED,"",0, TEMPERATE});
	}

	planet_builder_lock.lock();
	planet_builder_status = "Scanning " + std::to_string(n_biomes) + " Biomes";
	planet_builder_lock.unlock();
	// Iterate the world and place each world block into the closest biome
	for (int y=0; y<WORLD_HEIGHT; ++y) {
		for (int x = 0; x<WORLD_WIDTH; ++x) {
			int distance = std::numeric_limits<int>::max();
			int closest_index = -1;

			for (int i=0; i<n_biomes; ++i) {
				const int biome_x = centroids[i].first;
				const int biome_y = centroids[i].second;
				const int dx = std::abs(biome_x - x);
				const int dy = std::abs(biome_y - y);
				const int biome_distance = (dx*dx) + (dy*dy);
				if (biome_distance < distance) {
					distance = biome_distance;
					closest_index = i;
				}
			}

			planet.landblocks[planet.idx(x,y)].biome_idx = closest_index;
		}
		planet_display_update_initial(planet);
	}

	// Build each biome
	planet_builder_lock.lock();
	planet_builder_status = "Hand-crafting fjords";
	planet_builder_lock.unlock();

	for (int i=0; i<n_biomes; ++i) {
		// Find: total # of each type in biome, average latitude (y coordinate), biome extents
		int n_cells = 0;
		int n_water = 0;
		int n_plains = 0;
		int n_hills = 0;
		int n_mountains = 0;
		int total_latitude = 0;

		for (int y=0; y<WORLD_HEIGHT; ++y) {
			for (int x = 0; x<WORLD_WIDTH; ++x) {
				const int cell_biome_idx = planet.landblocks[planet.idx(x,y)].biome_idx;
				if (cell_biome_idx == i) {
					++n_cells;
					switch (planet.landblocks[planet.idx(x,y)].type) {
						case WATER : ++n_water; break;
						case PLAINS : ++n_plains; break;
						case HILLS : ++n_hills; break;
						case MOUNTAINS : ++n_mountains; break;
					}
					total_latitude += y;
				}
			}
		}
		if (n_cells > 0) {
			const int average_latitude = total_latitude / n_cells;
			// Determine mean temperature
			const double distance_from_equator = 1.0 - ((std::abs( (WORLD_HEIGHT/2.0) - average_latitude ))/(WORLD_HEIGHT/1.8));
			planet.biomes[i].mean_temperature = -89 + (distance_from_equator*147) + rng.roll_dice(1,10) - rng.roll_dice(1,10);

			if (planet.biomes[i].mean_temperature < -20) {
				planet.biomes[i].climate = ARCTIC;
			} else if (planet.biomes[i].mean_temperature < 0) {
				planet.biomes[i].climate = SUBARCTIC;
			} else if (planet.biomes[i].mean_temperature < 5) {
				planet.biomes[i].climate = COLD;
			} else if (planet.biomes[i].mean_temperature < 10) {
				planet.biomes[i].climate = TEMPERATE;
			} else if (planet.biomes[i].mean_temperature < 25) {
				planet.biomes[i].climate = WARM;
			} else if (planet.biomes[i].mean_temperature < 45) {
				planet.biomes[i].climate = HOT;
			} else {
				planet.biomes[i].climate = SCORCHING;
			}

			// Determine dominant feature
			// Use that and climate to determine a biome type
			if (n_water > n_plains && n_water > n_hills && n_water > n_mountains) {
				if (planet.biomes[i].climate == ARCTIC) {
					planet.biomes[i].biome_type = GLACIER;
				} else if (planet.biomes[i].climate == SUBARCTIC) {
					planet.biomes[i].biome_type = ICY_SEA;
				} else if (planet.biomes[i].climate == SCORCHING) {
					planet.biomes[i].biome_type = DUST_SEA;
				} else {
					planet.biomes[i].biome_type = OCEAN;
				}

			} else if (n_plains > n_water && n_plains > n_hills && n_plains > n_mountains) {
				if (planet.biomes[i].climate == ARCTIC) {
					planet.biomes[i].biome_type = TUNDRA;
				} else if (planet.biomes[i].climate == SUBARCTIC) {
					int random = rng.roll_dice(1,6);
					if (random < 3) {
						planet.biomes[i].biome_type = TUNDRA;
					} else {
						planet.biomes[i].biome_type = SWAMP;
					}
				} else if (planet.biomes[i].climate == COLD) {
					int random = rng.roll_dice(1,6);
					if (random == 1) {
						planet.biomes[i].biome_type = SWAMP;
					} else if (random == 2) {
						planet.biomes[i].biome_type = WOODS;
					} else if (random == 3) {
						planet.biomes[i].biome_type = FOREST;
					} else {
						planet.biomes[i].biome_type = FLATLAND;
					}
				} else if (planet.biomes[i].climate == TEMPERATE || planet.biomes[i].climate == WARM) {
					int random = rng.roll_dice(1,7);
					if (random == 1 || random == 2) {
						planet.biomes[i].biome_type = SWAMP;
					} else if (random == 3 || random == 4) {
						planet.biomes[i].biome_type = WOODS;
					} else if (random == 5 || random == 6) {
						planet.biomes[i].biome_type = FOREST;
					} else {
						planet.biomes[i].biome_type = FLATLAND;
					}
				} else if (planet.biomes[i].climate == HOT) {
					int random = rng.roll_dice(1,8);
					if (random == 1) {
						planet.biomes[i].biome_type = SWAMP;
					} else if (random == 2) {
						planet.biomes[i].biome_type = WOODS;
					} else if (random == 3) {
						planet.biomes[i].biome_type = FLATLAND;
					} else if (random == 4 || random == 5) {
						planet.biomes[i].biome_type = JUNGLE;
					} else if (random == 6 || random == 7) {
						planet.biomes[i].biome_type = DESERT;
					} else {
						planet.biomes[i].biome_type = BADLANDS;
					} 
				} else if (planet.biomes[i].climate == SCORCHING) {
					int random = rng.roll_dice(1,6);
					if (random == 1) {
						planet.biomes[i].biome_type = FLATLAND;
					} else if (random == 2) {
						planet.biomes[i].biome_type = JUNGLE;
					} else if (random == 3 || random == 4) {
						planet.biomes[i].biome_type = DESERT;
					} else {
						planet.biomes[i].biome_type = BADLANDS;
					} 
				}
			} else if (n_hills > n_water && n_hills > n_plains && n_hills > n_mountains) {
				if (planet.biomes[i].climate == ARCTIC) {
					planet.biomes[i].biome_type = HIGH_TUNDRA;
				} else if (planet.biomes[i].climate == SUBARCTIC) {
					int random = rng.roll_dice(1,2);
					if (random == 1) {
						planet.biomes[i].biome_type = HIGH_TUNDRA;
					} else {
						planet.biomes[i].biome_type = HILL_BOG;
					}
				} else if (planet.biomes[i].climate == COLD) {
					int random = rng.roll_dice(1,4);
					if (random == 1) {
						planet.biomes[i].biome_type = HIGH_TUNDRA;
					} else if (random == 2) {
						planet.biomes[i].biome_type = WOODS;
					} else if (random == 3) {
						planet.biomes[i].biome_type = FOREST;
					} else {
						planet.biomes[i].biome_type = HILL_BOG;
					}
				} else if (planet.biomes[i].climate == TEMPERATE || planet.biomes[i].climate == WARM) {
					int random = rng.roll_dice(1,7);
					if (random == 1 || random == 2) {
						planet.biomes[i].biome_type = HILL_BOG;
					} else if (random == 3 || random == 4) {
						planet.biomes[i].biome_type = WOODS;
					} else if (random == 5 || random == 6) {
						planet.biomes[i].biome_type = FOREST;
					} else {
						planet.biomes[i].biome_type = FLATLAND;
					}
				} else if (planet.biomes[i].climate == HOT) {
					int random = rng.roll_dice(1,8);
					if (random == 1) {
						planet.biomes[i].biome_type = HILL_BOG;
					} else if (random == 2) {
						planet.biomes[i].biome_type = WOODS;
					} else if (random == 3) {
						planet.biomes[i].biome_type = FLATLAND;
					} else if (random == 4 || random == 5) {
						planet.biomes[i].biome_type = JUNGLE;
					} else if (random == 6 || random == 7) {
						planet.biomes[i].biome_type = HIGH_DESERT;
					} else {
						planet.biomes[i].biome_type = HIGH_BADLANDS;
					} 
				} else if (planet.biomes[i].climate == SCORCHING) {
					int random = rng.roll_dice(1,6);
					if (random == 1) {
						planet.biomes[i].biome_type = FLATLAND;
					} else if (random == 2) {
						planet.biomes[i].biome_type = JUNGLE;
					} else if (random == 3 || random == 4) {
						planet.biomes[i].biome_type = HIGH_DESERT;
					} else {
						planet.biomes[i].biome_type = HIGH_BADLANDS;
					} 
				}
			} else {
				if (planet.biomes[i].climate == ARCTIC || planet.biomes[i].climate == SUBARCTIC || planet.biomes[i].climate == COLD) {
					planet.biomes[i].biome_type = HIGH_TUNDRA;
				} else if (planet.biomes[i].climate == TEMPERATE || planet.biomes[i].climate == WARM || planet.biomes[i].climate == HOT) {
					planet.biomes[i].biome_type = NORMAL_MOUNTAINS;
				} else if (planet.biomes[i].climate == SCORCHING) {
					planet.biomes[i].biome_type = HIGH_BADLANDS;
				}
			}

			planet.biomes[i].name = name_biome(planet.biomes[i]);

			planet_display_update_initial(planet);
			planet_builder_lock.lock();
			planet_builder_status = planet.biomes[i].name;
			planet_builder_lock.unlock();
		}
	}
}

void builder_save_planet(planet_t &planet) {
	planet_builder_lock.lock();
	planet_builder_status = "Saving the world. To disk, sadly.";
	planet_builder_lock.unlock();
	save_planet(planet);
	//std::this_thread::sleep_for(std::chrono::seconds(10));
}

std::pair<int,int> builder_select_starting_region(planet_t &planet, const int min, const int max) {
	int start_x = WORLD_WIDTH / 2;
	int start_y = WORLD_HEIGHT / 2;


	planet_builder_lock.lock();
	planet_builder_status = "Picking a comfortable place to crash-land";
	planet_builder_lock.unlock();

	for (int x=planet_build_term_width/2; x<WORLD_WIDTH-(planet_build_term_width/2); ++x) {
		planet_display_update_regional(planet, x, start_y, min, max);
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}

	//std::this_thread::sleep_for(std::chrono::seconds(10));
	while (planet.landblocks[planet.idx(start_x, start_y)].type != PLAINS) {
		--start_x;
	}
	return std::make_pair(start_x, start_y);
}

std::pair<uint8_t, int> planet_builder_soil_height(const biome_t &biome, random_number_generator &rng) {
	switch (biome.biome_type) {
		case ICY_SEA: return std::make_pair(tile_type::SOIL, rng.roll_dice(1,6));
		case DUST_SEA: return std::make_pair(tile_type::YELLOW_SAND, rng.roll_dice(1,4));
		case OCEAN: return std::make_pair(tile_type::SOIL, rng.roll_dice(1,10));
		case TUNDRA: return std::make_pair(tile_type::SOIL, rng.roll_dice(1,3));
		case SWAMP: return std::make_pair(tile_type::SOIL, rng.roll_dice(3,6));
		case FLATLAND: return std::make_pair(tile_type::SOIL, rng.roll_dice(2,4));
		case WOODS: return std::make_pair(tile_type::SOIL, rng.roll_dice(2,6));
		case FOREST: return std::make_pair(tile_type::SOIL, rng.roll_dice(3,6));
		case JUNGLE: return std::make_pair(tile_type::SOIL, rng.roll_dice(3,6));
		case HIGH_TUNDRA: return std::make_pair(tile_type::SOIL, 1);
		case HILL_BOG: return std::make_pair(tile_type::SOIL, rng.roll_dice(3,6));
		case DESERT: return std::make_pair(tile_type::YELLOW_SAND, rng.roll_dice(2,4));
		case BADLANDS: return std::make_pair(tile_type::RED_SAND, rng.roll_dice(1,3));
		case HIGH_DESERT: return std::make_pair(tile_type::YELLOW_SAND, 1);
		case HIGH_BADLANDS: return std::make_pair(tile_type::RED_SAND, 1);
		default: return std::make_pair(tile_type::SOIL, 1);
	}
}

uint16_t planet_builder_covering(const biome_t &biome, random_number_generator &rng, const bool is_water) {
	if (is_water) return tile_content::NOTHING;

	if (biome.biome_type == ICY_SEA || biome.biome_type == TUNDRA || biome.biome_type == HIGH_TUNDRA) {
		int roll = rng.roll_dice(1,8);
		switch (roll) {
			case 1 : return tile_content::PERMAFROST_BLUE;
			case 3 : return tile_content::PERMAFROST_CRACKED;
			case 4 : return tile_content::PERMAFROST_DIRTY;
			case 5 : return tile_content::LYCHEN;
			case 6 : return tile_content::MOSS;
			default : return tile_content::PERMAFROST_WHITE;
		}
	} else if (biome.biome_type == DESERT || biome.biome_type == DUST_SEA || biome.biome_type == HIGH_DESERT) {
		int roll = rng.roll_dice(1,20);
		switch (roll) {
			case 1 : return tile_content::YELLOW_SAND_CRACKED;
			case 2 : return tile_content::YELLOW_SAND_DIRTY;
			case 3 : return tile_content::RED_SAND;
			case 4 : return tile_content::RED_SAND_CRACKED;
			case 5 : return tile_content::CACTUS;
			case 6 : return tile_content::PEBBLES;
			default : return tile_content::NOTHING;
		}
	} else if (biome.biome_type == BADLANDS || biome.biome_type == HIGH_BADLANDS) {
		int roll = rng.roll_dice(1,40);
		switch (roll) {
			case 1 : return tile_content::YELLOW_SAND_CRACKED;
			case 2 : return tile_content::YELLOW_SAND_DIRTY;
			case 3 : return tile_content::RED_SAND;
			case 4 : return tile_content::RED_SAND_CRACKED;
			case 5 : return tile_content::CACTUS;
			case 6 : return tile_content::PEBBLES;
			default : return tile_content::NOTHING;
		}
	} else if (biome.biome_type == SWAMP) {
		int roll = rng.roll_dice(1,10);
		switch (roll) {
			case 1 : return tile_content::WHITE_SAND;
			case 2 : return tile_content::GRAVEL;
			case 3 : return tile_content::MUD;
			case 4 : return tile_content::NOTHING;
			case 5 : return tile_content::REEDS;
			case 6 : return tile_content::MOSS;
			case 7 : return tile_content::GORSE;
			case 8 : return tile_content::GRASS;
			case 9 : return tile_content::GRASS_SPARSE;
			default : return tile_content::NOTHING;
		}
	} else if (biome.biome_type == FLATLAND || biome.biome_type == OCEAN || biome.biome_type == WOODS || biome.biome_type == FOREST || biome.biome_type == JUNGLE) {
		int roll = rng.roll_dice(1,10);
		switch (roll) {
			case 1 : return tile_content::WILDFLOWER;
			case 2 : return tile_content::GRAVEL;
			case 3 : return tile_content::MUD;
			case 4 : return tile_content::GRASS_SPARSE;
			case 5 : return tile_content::REEDS;
			case 6 : return tile_content::THISTLE;
			case 7 : return tile_content::HEATHER;
			case 8 : return tile_content::SHRUB;
			default : return tile_content::GRASS;
		}
	}

	return tile_content::NOTHING;
}

void build_region(planet_t &planet, std::pair<int,int> location, random_number_generator &rng, bool has_crash_site=true) {
	const biome_t biome = planet.biomes[planet.landblocks[planet.idx(location.first, location.second)].biome_idx];
	const std::string region_name = biome.name;

	planet_builder_lock.lock();
	planet_builder_status = "Scanning " + region_name + "- altitude";
	planet_builder_lock.unlock();

	const double region_noise_x = (double)location.first / WORLD_PERLIN_DIVISOR;
	const double region_noise_y = (double)location.second / WORLD_PERLIN_DIVISOR;
	const double region_step_x = 1.0 / WORLD_PERLIN_DIVISOR * (double)REGION_WIDTH;
	const double region_step_y = 1.0 / WORLD_PERLIN_DIVISOR * (double)REGION_HEIGHT;

	// Prime the noise function, and extrapolate the region's noise into a height map
	perlin_noise mountains(planet.perlin_seed);
	perlin_noise flatlands(std::numeric_limits<int>::max()-planet.perlin_seed);
	perlin_noise mixer(std::numeric_limits<int>::max()+planet.perlin_seed);

	std::vector<int> height_map(REGION_WIDTH * REGION_HEIGHT);
	for (int y=0; y<REGION_HEIGHT; ++y) {
		const double Y = region_noise_y + ((double)y * region_step_y);
		for (int x=0; x<REGION_WIDTH; ++x) {
			const double X = region_noise_x + ((double)x * region_step_x);

			const double mtn_noise_pixel = mountains.noise(X*WORLD_MOUNTAIN_MULTIPLIER, Y*WORLD_MOUNTAIN_MULTIPLIER, 0.8) * 300.0;
			const double flat_noise_pixel = flatlands.noise(X, Y, 0.8) * 20.0;
			const double mtn_pct = mixer.noise(X/10.0,Y/10.0,0.8);
			const double flat_pct = 1.0 - mtn_pct;

			const double height = (mtn_noise_pixel * mtn_pct) + (flat_noise_pixel * flat_pct);
			height_map[(REGION_WIDTH * y) + x] = height;
		}
	}	

	planet_builder_lock.lock();
	planet_builder_status = "Roughing up the edges a bit";
	planet_builder_lock.unlock();

	// TODO: Additional noise for hill and mountain regions

	planet_builder_lock.lock();
	planet_builder_status = "Applying spit and polish";
	planet_builder_lock.unlock();
	// Smoothing
	for (int i=0; i<10; ++i) {
		std::vector<int> smoothed_height_map = height_map;
		for (int y=1; y<REGION_WIDTH-1; ++y) {
			for (int x=1; x<REGION_WIDTH-1; ++x) {
				int total = height_map[((y-1)*REGION_WIDTH)+x-1] +
					height_map[((y-1)*REGION_WIDTH)+x] +
					height_map[((y-1)*REGION_WIDTH)+x+1] +
					height_map[(y*REGION_WIDTH)+x-1] +
					height_map[(y*REGION_WIDTH)+x] +
					height_map[(y*REGION_WIDTH)+x+1] +
					height_map[((y+1)*REGION_WIDTH)+x-1] +
					height_map[((y+1)*REGION_WIDTH)+x] +
					height_map[((y+1)*REGION_WIDTH)+x+1];
				smoothed_height_map[(y*REGION_WIDTH)+x] = total/9;
			}
		}
		height_map = smoothed_height_map;
	}

	planet_builder_lock.lock();
	planet_builder_status = "Laying down strata";
	planet_builder_lock.unlock();
	// Divide everything by 30!
	int max = std::numeric_limits<int>::min();
	int min = std::numeric_limits<int>::max();
	for (int &n : height_map) {
		n /= 30;
		if (n > max) max = n;
		if (n < min) min = n;
	}

	// Start laying down surface layers
	region_t region;
	region.region_x = location.first;
	region.region_y = location.second;
	region.biome_idx = planet.landblocks[planet.idx(location.first, location.second)].biome_idx;

	const std::pair<uint8_t,int> soil_height = planet_builder_soil_height(biome, rng);

	for (int y=0; y<REGION_HEIGHT; ++y) {
		for (int x=0; x<REGION_WIDTH; ++x) {
			// The bottom layer is *always* special solid rock.
			region.set(x,y,0, tile_type::SEMI_MOLTEN_ROCK, tile_content::NOTHING, 0, 0, true);

			// Fill rock under the ground
			const int ground_height = 64+height_map[(y*REGION_WIDTH)+x];
			for (int z=1; z<ground_height - soil_height.second; ++z) {
				region.set(x,y,z, tile_type::ROCK, tile_content::NOTHING, 0, 0, true);
			}
			for (int z=ground_height-soil_height.second; z<ground_height; ++z) {
				region.set(x,y,z, soil_height.first, tile_content::NOTHING, 0, 0, true);
			}

			// TODO: Pick an appropriate surface tile here
			region.set(x,y,ground_height, soil_height.first, planet_builder_covering(biome, rng, ground_height < planet.water_height/30), 0, 0, false);

			// Fill in the sky
			for (int z=ground_height+1; z<REGION_DEPTH; ++z) {
				region.set(x,y,z, tile_type::NOTHING, tile_content::NOTHING, 0, 0, false);
			}
		}
	}

	// Trees will go here
	planet_builder_lock.lock();
	planet_builder_status = "Planting trees";
	planet_builder_lock.unlock();

	// Crash site
	planet_builder_lock.lock();
	planet_builder_status = "Crashing the space ship";
	planet_builder_lock.unlock();

	// Control components
	auto camera = create_entity()
		->assign(world_position_t{location.first, location.second, REGION_WIDTH/2, REGION_HEIGHT/2, 64})
		->assign(calendar_t{});

	// Settler building
	planet_builder_lock.lock();
	planet_builder_status = "Determining initial local populations";
	planet_builder_lock.unlock();

	// Initial inventory

	// Save the region
	planet_builder_lock.lock();
	planet_builder_status = "Saving " + region_name + " to disk";
	planet_builder_lock.unlock();
	save_region(region);
	const std::string save_filename = "world/savegame.dat";
	std::fstream lbfile(save_filename, std::ios::out | std::ios::binary);
	Poco::DeflatingOutputStream deflate(lbfile, Poco::DeflatingStreamBuf::STREAM_GZIP);
	ecs_save(deflate);
}

void build_planet() {
	planet_build_done.store(false);

	bool ok = false;

	random_number_generator rng;
	planet_t planet;
	int max = std::numeric_limits<int>::min();
	int min = std::numeric_limits<int>::max();
	while (!ok) {
		planet.rng_seed = rng.initial_seed;
		const int perlin_seed = rng.roll_dice(1, std::numeric_limits<int>::max());
		planet.perlin_seed = perlin_seed;

		// Make a zero-height map
		planet_zero_fill(planet);

		// Noise-based world map
		planet_noise_map(planet, perlin_seed);

		// Divide types by height
		planet_base_type_allocation(planet);

		// Check for validity
		for (block_t &block : planet.landblocks) {
			if (block.height > max) max = block.height;
			if (block.height < min) min = block.height;
		}
		//std::cout << min << ".." << max << "\n";
		if (max < 1000) {
			ok = true;
		} else {
			max = std::numeric_limits<int>::min();
			min = std::numeric_limits<int>::max();
		}

	}

	// Make a biome map
	build_biomes(planet, rng);

	// Save it to disk
	builder_save_planet(planet);

	// Select a crash site
	auto crash_site = builder_select_starting_region(planet, min, max);

	// Materialize this region
	build_region(planet, crash_site, rng);

	planet_build_done.store(true);
}
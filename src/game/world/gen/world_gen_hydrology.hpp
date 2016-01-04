#pragma once

#include "world_height_map.hpp"
#include <vector>
#include <cstdint>
#include <memory>
#include "../../../engine/rng.hpp"

typedef std::vector<uint8_t> rain_map_t;
typedef std::vector<int16_t> temperature_map_t;
typedef std::vector<uint8_t> water_level_map_t;

std::unique_ptr<rain_map_t> get_rain_map ( heightmap_t * heightmap );
std::unique_ptr<temperature_map_t> get_temperature_map ( heightmap_t * heightmap );
std::unique_ptr<water_level_map_t> perform_hydrology ( heightmap_t * heightmap, engine::random_number_generator * rng );

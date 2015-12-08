#pragma once

#include <cstdint>

constexpr uint8_t WORLD_WIDTH = 16;
constexpr uint8_t WORLD_HEIGHT = 16;
constexpr uint16_t WORLD_TILES = WORLD_HEIGHT * WORLD_WIDTH;

constexpr uint8_t REGION_WIDTH = 254;
constexpr uint8_t REGION_HEIGHT = 254;
constexpr uint16_t REGION_TILES = REGION_HEIGHT * REGION_WIDTH;

constexpr int NUMBER_OF_TILES_IN_THE_WORLD = (WORLD_WIDTH * REGION_WIDTH) * (WORLD_HEIGHT * REGION_HEIGHT);

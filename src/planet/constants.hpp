#pragma once

#include <cstdint>

constexpr int WORLD_HEIGHT = 128;
constexpr int WORLD_WIDTH = 128;
constexpr int WORLD_TILES_COUNT = WORLD_HEIGHT * WORLD_WIDTH;

constexpr int REGION_WIDTH=256;
constexpr int REGION_HEIGHT=256;
constexpr int REGION_DEPTH=128;
constexpr int REGION_TILES_COUNT = REGION_WIDTH * REGION_HEIGHT * REGION_DEPTH;

namespace tile_flags
{
	using tile_flag_type = uint32_t;

	constexpr tile_flag_type CAN_GO_UP = 1;
	constexpr tile_flag_type CAN_GO_DOWN = 2;
	constexpr tile_flag_type CAN_GO_NORTH = 4;
	constexpr tile_flag_type CAN_GO_EAST = 8;
	constexpr tile_flag_type CAN_GO_SOUTH = 16;
	constexpr tile_flag_type CAN_GO_WEST = 32;
	constexpr tile_flag_type CAN_STAND_HERE = 64;
	constexpr tile_flag_type CONSTRUCTION = 128;
	constexpr tile_flag_type REVEALED = 256;
	constexpr tile_flag_type VISIBLE = 512;
	constexpr tile_flag_type SOLID = 1024;
	constexpr tile_flag_type ABOVE_GROUND = 2048;
	constexpr tile_flag_type BLOODSTAIN = 4096;
	constexpr tile_flag_type CAN_GO_NORTH_WEST = 8192;
	constexpr tile_flag_type CAN_GO_NORTH_EAST = 16384;
	constexpr tile_flag_type CAN_GO_SOUTH_WEST = 32768;
	constexpr tile_flag_type CAN_GO_SOUTH_EAST = 65536;
	constexpr tile_flag_type OPAQUE_TILE = 131072;
}

namespace tile_type {
	constexpr uint8_t SEMI_MOLTEN_ROCK = 0;
	constexpr uint8_t SOLID = 1;
	constexpr uint8_t OPEN_SPACE = 2;
	constexpr uint8_t WALL = 3;
	constexpr uint8_t RAMP = 4;
	constexpr uint8_t STAIRS_UP = 5;
	constexpr uint8_t STAIRS_DOWN = 6;
	constexpr uint8_t STAIRS_UPDOWN = 7;
	constexpr uint8_t FLOOR = 8;
	constexpr uint8_t TREE_TRUNK = 9;
	constexpr uint8_t TREE_LEAF = 10;
	constexpr uint8_t WINDOW = 11;
	constexpr uint8_t CLOSED_DOOR = 12;
}
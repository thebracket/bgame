#pragma once

#include "tile_types.hpp"
#include <Poco/DeflatingStream.h>
#include <Poco/InflatingStream.h>
#include "../../engine/virtual_terminal.hpp"
#include <bitset>

enum TILE_OPTIONS
{
	WALK_BLOCKED = 1,
	VIEW_BLOCKED = 2,
	CAN_STAND_HERE = 3,
	CAN_GO_NORTH = 4,
	CAN_GO_SOUTH = 5,
	CAN_GO_EAST = 6,
	CAN_GO_WEST = 7,
	CAN_GO_UP = 8,
	CAN_GO_DOWN = 9,
	SOLID = 10
};

constexpr int TILE_OPTIONS_COUNT = 11;

class tile_t
{
public:
	tile_t()
	{
		flags = 0L;
	}

	uint8_t water_level;
	unsigned char base_tile_type;
	unsigned char ground;
	unsigned char climate;
	unsigned char covering;
	std::bitset<TILE_OPTIONS_COUNT> flags;

	// Non-persistent helpers
	engine::vterm::screen_character render_as;

	void save(Poco::DeflatingOutputStream &lbfile);
	void load(Poco::InflatingInputStream &lbfile);
};

void tile_render_calculation(tile_t * tile);

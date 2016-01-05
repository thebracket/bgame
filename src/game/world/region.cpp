#include "region.hpp"
#include "../components/component_loader.h"
#include "tile_types.hpp"

void region_t::save(Poco::DeflatingOutputStream& lbfile)
{
	for (const bool &r : revealed)
	{
		save_primitive<bool>(lbfile, r);
	}
	for (tile_t &tile : tiles)
	{
		tile.save(lbfile);
	}
}

void region_t::load(Poco::InflatingInputStream& lbfile)
{
	revealed.clear();
	for (auto i = 0; i < REGION_TILES; ++i)
	{
		bool reveal;
		load_primitive<bool>(lbfile, reveal);
		revealed.push_back(reveal);
	}
	tiles.clear();
	for (int i = 0; i < REGION_TILES; ++i)
	{
		tile_t tile;
		tile.load(lbfile);
		tiles.push_back(tile);
	}
}

void test_walkability(int flag, tile_t * src, tile_t * dst)
{
	if (!dst->flags.test(TILE_OPTIONS::SOLID)
			and !dst->flags.test(TILE_OPTIONS::WALK_BLOCKED)
			and dst->water_level == 0)
	{
		src->flags.set(flag);
	}
}

void region_t::calculate_walkability()
{
	for (int z = 1; z < REGION_DEPTH - 1; ++z)
	{
		for (int y = 1; y < REGION_WIDTH - 1; ++y)
		{
			for (int x = 1; x < REGION_HEIGHT - 1; ++x)
			{
				tile_t * tile = &tiles[tile_idx(x, y, z)];

				if (tile->flags.test(TILE_OPTIONS::SOLID))
				{

					tile->flags.set(TILE_OPTIONS::VIEW_BLOCKED);
					tile->flags.set(TILE_OPTIONS::WALK_BLOCKED);
					const int above_idx = tile_idx(x, y, z + 1);
					tile_t * above_solid = &tiles[above_idx];
					if (!above_solid->flags.test(TILE_OPTIONS::SOLID))
					{
						above_solid->flags.set(TILE_OPTIONS::CAN_STAND_HERE);
					}
				}
				else
				{
					if (tile->base_tile_type == tile_type::RAMP)
					{
						// Ramps have the tile above them marked as walkable and gain the down option
						const int above_idx = tile_idx(x, y, z + 1);
						tile_t * above_solid = &tiles[above_idx];
						if (!above_solid->flags.test(TILE_OPTIONS::SOLID))
						{
							above_solid->flags.set(
									TILE_OPTIONS::CAN_STAND_HERE);
							above_solid->flags.set(TILE_OPTIONS::CAN_GO_DOWN);
							tile->flags.set(TILE_OPTIONS::CAN_GO_UP);
						}
					}

					// Test for NSEW travel
					const int north_idx = tile_idx(x, y - 1, z);
					const int south_idx = tile_idx(x, y + 1, z);
					const int east_idx = tile_idx(x + 1, y, z);
					const int west_idx = tile_idx(x - 1, y, z);

					test_walkability(TILE_OPTIONS::CAN_GO_NORTH, tile,
							&tiles[north_idx]);
					test_walkability(TILE_OPTIONS::CAN_GO_SOUTH, tile,
							&tiles[south_idx]);
					test_walkability(TILE_OPTIONS::CAN_GO_EAST, tile,
							&tiles[east_idx]);
					test_walkability(TILE_OPTIONS::CAN_GO_WEST, tile,
							&tiles[west_idx]);

				}
			}
		}
	}
}


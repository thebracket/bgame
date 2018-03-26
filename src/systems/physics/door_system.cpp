#include "stdafx.h"
#include "door_system.hpp"
#include "../../planet/region/region.hpp"
#include "../../global_assets/game_planet.hpp"
#include "../../global_assets/rng.hpp"
#include "../../render_engine/chunks/chunks.hpp"

using namespace bengine;
using namespace region;

namespace systems {
	namespace doors {

		bool dirty = true;

		void doors_changed() {
			dirty = true;
		}

		void run(const double &duration_ms) {
			if (dirty) {
				each<construct_door_t, position_t>([](entity_t &e, construct_door_t &door, position_t &pos) {
					const int idx = mapidx(pos.x, pos.y, pos.z);
					if (door.locked) {
						set_tile_type(idx, tile_type::CLOSED_DOOR);
					}
					else {
						set_tile_type(idx, tile_type::FLOOR);
					}

					tile_calculate(pos.x, pos.y, pos.z);
					for (int Z = -2; Z<3; ++Z) {
						for (int Y = -2; Y<3; ++Y) {
							for (int X = -2; X<3; ++X) {
								tile_calculate(pos.x + X, pos.y + Y, pos.z + Z);
							}
						}
					}
					chunks::mark_chunk_dirty_by_tileidx(idx);
				});
				dirty = false;
			}
		}
	}
}
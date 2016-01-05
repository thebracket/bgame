#include "global_illumination_system.hpp"
#include "../world/universe.hpp"
#include "../world/world_defs.hpp"

void global_illumination_system::tick(const double &duration_ms) {
	if (universe->globals.paused) return;

	region_t * region = get_current_region();

	for (int z=0; z<REGION_DEPTH; ++z) {
		for (int y=0; y<REGION_HEIGHT; ++y) {
			for (int x=0; x<REGION_WIDTH; ++x) {
				const int tile_idx = get_tile_index(x,y,z);
				region->tiles[tile_idx].light_color = { 1.0, 1.0, 1.0 };
			}
		}
	}
}

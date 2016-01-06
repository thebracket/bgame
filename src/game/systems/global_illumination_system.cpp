#include "global_illumination_system.hpp"
#include "../world/universe.hpp"
#include "../world/world_defs.hpp"
#include "../game.h"
#include <unordered_set>

void sunlight() {
	region_t * region = get_current_region();

	calendar_component * calendar = ECS->find_entity_component<calendar_component>( universe->globals.cordex_handle );
	position_component3d * camera_pos = get_camera_position();

	const int top_y = camera_pos->pos.y - ((768 / 8) / 2);
	const int bottom_y = camera_pos->pos.y + ((768 / 8) / 2);
	const int left_x = camera_pos->pos.x - ((1024 / 8) / 2);
	const int right_x = camera_pos->pos.x + ((1024 / 8) / 2);

	const float delta_z = 0.1F;
	const float delta_y = 0.1F;
	const float delta_x = (calendar->sun_x / 768.0F)*2.0;
	//std::cout << "Delta-X: " << delta_x << "\n";

	const float sun_intensity = 1.0F;

	for (int z=camera_pos->pos.z-10; z<camera_pos->pos.z+1; ++z) {
		for (int y=top_y; y<bottom_y; ++y) {
			for (int x=left_x; x<right_x; ++x) {
				const int tile_idx = get_tile_index(x,y,z);

				bool lit = false;
				bool blocked = false;
				bool clipped = false;
				float tile_x = x;
				float tile_y = y;
				float tile_z = z;
				int n = 0;
				int last_idx = -1;

				//std::cout << "Ray [" << delta_x << "," << delta_z << "]: ";
				while (!blocked and !clipped and n<20) {
					tile_x += delta_x;
					tile_y += delta_y;
					tile_z += delta_z;

					const int tx = std::round(tile_x);
					const int ty = std::round(tile_y);
					const int tz = std::round(tile_z);
					//std::cout << "{" << tx << "," << tz << "},";

					if (tx < 1 or tx >= REGION_WIDTH or tz < 1 or tz >= REGION_DEPTH or ty < 1 or ty > REGION_HEIGHT) {
						clipped = true;
						//std::cout << "C";
					} else {
						const int idx = get_tile_index(tx,ty,tz);
						if (idx != last_idx) {
							if (region->tiles[idx].flags.test(TILE_OPTIONS::VIEW_BLOCKED) or region->tiles[idx].flags.test(TILE_OPTIONS::SOLID) ) {
								blocked = true;
								//std::cout << "B";
							}
						}
						last_idx = idx;
					}
					++n;
				}
				if (!blocked) lit = true;
				//std::cout << "\n";

				if (lit) {
					if ( calendar->is_daytime ) {
						region->tiles[tile_idx].light_color = std::make_tuple( sun_intensity, sun_intensity, sun_intensity );
					} else {
						region->tiles[tile_idx].light_color = { 0.2, 0.2, 0.5 };
					}
				} else {
					region->tiles[tile_idx].light_color = { 0.3, 0.3, 0.3 };
				}
			}
		}
	}
}

void global_illumination_system::tick(const double &duration_ms) {
	if (universe->globals.paused) return;

	sunlight();
}

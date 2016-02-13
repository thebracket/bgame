#include "global_illumination_system.hpp"
#include "../world/universe.hpp"
#include "../world/world_defs.hpp"
#include "../game.h"
#include "../../engine/geometry.hpp"
#include <unordered_set>

using geometry::line_func_3d;
using geometry::project_angle;
using geometry::DEGRAD;

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
						region->tiles[tile_idx].light_color = std::make_tuple( 1.0F, 1.0F, 0.95F );
					} else {
<<<<<<< HEAD
						region->tiles[tile_idx].light_color = std::make_tuple( 0.25, 0.5, 0.9 );
=======
						region->tiles[tile_idx].light_color = { 0.25, 0.25, 0.25 };
>>>>>>> 87fbf38f61a1bddef7ba331fe16f72d61e72d852
					}
				} else {
					region->tiles[tile_idx].light_color = std::make_tuple( 0.5, 0.5, 0.5 );
				}
			}
		}
	}
}

void light_radius_for_visibility(point_light_component * view, const position_component3d * pos)
{
	constexpr double sweep_degrees_radians = 2 * DEGRAD;
	constexpr double three_sixy_degrees_radians = 360 * DEGRAD;

	view->last_visibility.clear();

	// You can always see yourself
	const int16_t x = pos->pos.x;
	const int16_t y = pos->pos.y;
	const uint8_t z = pos->pos.z;
	const uint8_t region_idx = pos->pos.region;
	const int my_idx = get_tile_index(x, y, z);
	const int radius = view->light_range;
	view->last_visibility.push_back(my_idx);
	region_t * current_region = world::planet->get_region(region_idx);

	// Sweep around
	for (double angle = 0; angle < three_sixy_degrees_radians; angle +=	sweep_degrees_radians)
	{
		double this_radius = radius;

		pair<int, int> destination = project_angle(x, y, this_radius, angle);
		for (int lz = z - radius; lz < z + radius; ++lz)
		{
			bool blocked = false;
			line_func_3d(x, y, z, destination.first, destination.second, lz,
					[&blocked,view,current_region] (int tx, int ty, int tz)
					{
						if (tx < 0 or tx > REGION_WIDTH or ty < 0 or ty > REGION_HEIGHT or tz<0 or tz>REGION_DEPTH) return;
						const int index = get_tile_index(tx,ty,tz);

						if (!blocked)
						{
							if (tx >=0 and tx < REGION_WIDTH and ty>=0 and ty<REGION_HEIGHT and tz>0 and tz<REGION_DEPTH)
							{
								view->last_visibility.push_back(index);
							}

							// FIXME: More block reasons
							tile_t * tile = &current_region->tiles[index];
							if (tile->flags.test( TILE_OPTIONS::VIEW_BLOCKED ) )
							{
								blocked = true;
							}
						}
					});
		}
	}

}

void point_lights() {
	bool forced = false;
	vector<lighting_changed_message> * change_list = game_engine->messaging->get_messages_by_type<lighting_changed_message>();
	for (lighting_changed_message &msg : *change_list)
	{
		if (!msg.deleted) forced = true;
		msg.deleted = true;
	}

	// Run a light view for every system that has one
	vector<point_light_component> * lights = ECS->find_components_by_type<point_light_component>();
	int i=0;
	for (point_light_component &light : *lights)
	{
		++i;
		position_component3d * pos = ECS->find_entity_component<position_component3d>(light.entity_id);
		region_t * region = world::planet->get_region(pos->pos.region);
		if (region == nullptr)
		{
			std::cout << "Danger! Region null for a light component!\n";
		}

		if (light.last_visibility.empty() or forced==true)
		{
			std::cout << "Light " << i << " recalculated. " << light.red << "/" << light.green << "/" << light.blue << "\n";
			light_radius_for_visibility(&light, pos);
			std::cout << light.last_visibility.size() << " lit tiles.\n";
		}

		// Apply the cached lighting shed
		for (const int &idx : light.last_visibility)
		{
			region->tiles[idx].light_color = std::make_tuple(light.red, light.green, light.blue);
		}
	}
}

void global_illumination_system::tick(const double &duration_ms) {
	//if (universe->globals.paused and has_run==false) return;
	has_run = true;

	sunlight();
	point_lights();
}

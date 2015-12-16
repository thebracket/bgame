#include "viewshed_system.h"
#include "../game.h"
#include "../world/planet.hpp"

using geometry::line_func_3d;
using geometry::project_angle;

void viewshed_system::tick ( const double &duration_ms ) {
    reset_visibility();

    // Run a viewshed for every system that has one
    vector<viewshed_component> * viewsheds = game_engine->ecs->find_components_by_type<viewshed_component> ();
    for (viewshed_component &viewshed : *viewsheds) {
        position_component3d * pos = game_engine->ecs->find_entity_component<position_component3d>(viewshed.entity_id);
	region_t * region = world::planet->get_region( pos->pos.region );
	if ( region == nullptr ) {
	    std::cout << "Danger! Region null for a viewshed component!\n";
	}

        if (pos->moved or viewshed.last_visibility.empty()) {
            switch (viewshed.scanner_type) {
            case visibility :
                scan_radius_for_visibility(&viewshed, pos);
                break;
            case penetrating :
                scan_radius_penetrating(&viewshed, pos);
                break;
            default :
                std::cout << "No scanner type found!\n";
            }
        }

        // Apply the cached viewshed
        for (const int &idx : viewshed.last_visibility) {
            region->visible[idx] = true;
        }
    }
}


void viewshed_system::reset_visibility() {
    position_component3d * camera_pos = game_engine->ecs->find_entity_component<position_component3d>(world::camera_handle);
    const uint8_t region_idx = camera_pos->pos.region;
    std::fill(world::planet->get_region( region_idx )->visible.begin(), world::planet->get_region( region_idx )->visible.end(), false);
}


bool viewshed_system::is_facing_this_way ( const position_component3d* pos, const double& angle )
{  
    //constexpr double ZERO = 0;
    constexpr double FORTY_FIVE = 45.0 * DEGRAD;
    //constexpr double NINETY = 90.0 * DEGRAD;
    constexpr double ONE_THIRTY_FIVE = 135.0 * DEGRAD;
    //constexpr double ONE_EIGHTY = 180.0 * DEGRAD;
    constexpr double TWO_TWENTY_FIVE = 225.0 * DEGRAD;
    //constexpr double TWO_SEVENTY = 270.0 * DEGRAD;
    constexpr double THREE_FIFTEEN = 315.0 * DEGRAD;
    //constexpr double THREE_SIXTY = 360.0 * DEGRAD;

    if (pos->facing == OMNI) return true;
    
    if (pos->facing == NORTH and (angle > TWO_TWENTY_FIVE and angle < THREE_FIFTEEN)) return true;    
    if (pos->facing == SOUTH and (angle > FORTY_FIVE and angle < ONE_THIRTY_FIVE)) return true;
    
    if (pos->facing == EAST and (angle > THREE_FIFTEEN or angle < FORTY_FIVE)) return true;
    if (pos->facing == WEST and (angle > ONE_THIRTY_FIVE and angle < TWO_TWENTY_FIVE)) return true;
    
    return false;
}

void viewshed_system::scan_radius_for_visibility(viewshed_component * view, const position_component3d * pos) {
    constexpr double sweep_degrees_radians = 2 * DEGRAD;
    constexpr double three_sixy_degrees_radians = 360 * DEGRAD;

    view->last_visibility.clear();

    // You can always see yourself
    const int16_t x = pos->pos.x;
    const int16_t y = pos->pos.y;
    const uint8_t z = pos->pos.z;
    const uint8_t region_idx = pos->pos.region;
    const int my_idx = get_tile_index(x,y,z);
    const int radius = view->scanner_range;
    view->last_visibility.push_back(my_idx);
    region_t * current_region = world::planet->get_region( region_idx );

    // Sweep around
    for ( double angle=0; angle<three_sixy_degrees_radians; angle+=sweep_degrees_radians ) {
	double this_radius = radius;
	if (!is_facing_this_way(pos, angle)) this_radius = radius/4;
	
        pair<int,int> destination = project_angle ( x, y, this_radius, angle );
	for ( int lz = z-radius; lz<z+radius; ++lz ) {
	    bool blocked = false;
	    line_func_3d ( x, y, z, destination.first, destination.second, lz, [&blocked,view,current_region] (int tx, int ty, int tz) {
		if (tx < 0 or tx > REGION_WIDTH or ty < 0 or ty > REGION_HEIGHT or tz<0 or tz>REGION_DEPTH) return;
		const int index = get_tile_index(tx,ty,tz);

		if (!blocked) {
		    if (tx >=0 and tx < REGION_WIDTH and ty>=0 and ty<REGION_HEIGHT and tz>0 and tz<REGION_DEPTH) {
			view->last_visibility.push_back(index);
			current_region->revealed[index] = true;
		    }

		    // FIXME: More block reasons
		    if (world::view_blocked_3d[index]) {
			blocked = true;
		    }
		}
	    });
	}
    }

}

void viewshed_system::scan_radius_penetrating(viewshed_component * view, const position_component3d * pos) {
    constexpr double sweep_degrees_radians = 2 * DEGRAD;
    constexpr double three_sixy_degrees_radians = 360 * DEGRAD;

    view->last_visibility.clear();

    // You can always see yourself
    const int16_t x = pos->pos.x;
    const int16_t y = pos->pos.y;
    const uint8_t z = pos->pos.z;
    const uint8_t region_idx = pos->pos.region;
    region_t * current_region = world::planet->get_region( region_idx );
    const double radius = view->scanner_range;
    view->last_visibility.push_back(get_tile_index(x, y, z));

    // Sweep around
    for ( double angle=0; angle<three_sixy_degrees_radians; angle+=sweep_degrees_radians ) {
	double this_radius = radius;
	if (!is_facing_this_way(pos, angle) == true) this_radius = 2.0;
	
        pair<int,int> destination = project_angle ( x, y, this_radius, angle );
	for ( int lz = z-this_radius; lz<z+this_radius; ++lz ) {
	    line_func_3d ( destination.first, destination.second, lz, x, y, z, [view,current_region] (int tx, int ty, int tz) {
		const int index = get_tile_index(tx,ty,tz);
		if (tx >=0 and tx < REGION_WIDTH and ty>=0 and ty<REGION_HEIGHT and tz>0 and tz<REGION_DEPTH) {
		    view->last_visibility.push_back(index);
		    current_region->revealed[index] = true;
		}
	    });
	}
    }
}

#include "viewshed_system.h"
#include "../../engine/globals.h"

void viewshed_system::tick ( const double &duration_ms ) {
    reset_visibility();

    // Run a viewshed for every system that has one
    vector<viewshed_component> * viewsheds = engine::globals::ecs->find_components_by_type<viewshed_component> ();
    for (viewshed_component &viewshed : *viewsheds) {
        position_component * pos = engine::globals::ecs->find_entity_component<position_component>(viewshed.entity_id);

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
            world::current_region->visible[idx] = true;
        }
    }
}


void viewshed_system::reset_visibility() {
    std::fill(world::current_region->visible.begin(), world::current_region->visible.end(), false);
}


bool viewshed_system::is_facing_this_way ( const position_component* pos, const double& angle )
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

void viewshed_system::scan_radius_for_visibility(viewshed_component * view, const position_component * pos) {
    constexpr double sweep_degrees_radians = 1 * DEGRAD;
    constexpr double three_sixy_degrees_radians = 360 * DEGRAD;

    view->last_visibility.clear();

    // You can always see yourself
    const int x = pos->x;
    const int y = pos->y;
    const int my_idx = world::current_region->idx(x,y);
    const unsigned char level_band = world::current_region->tiles[my_idx].level_band;
    const int radius = view->scanner_range;
    view->last_visibility.push_back(my_idx);

    // Sweep around
    for ( double angle=0; angle<three_sixy_degrees_radians; angle+=sweep_degrees_radians ) {
	double this_radius = radius;
	if (!is_facing_this_way(pos, angle)) this_radius = radius/4;
        pair<int,int> destination = project_angle ( x, y, this_radius, angle );
        bool blocked = false;
        line_func ( x, y, destination.first, destination.second, [&blocked,view,level_band] (int tx, int ty) {
            if (tx < 0 or tx > landblock_width or ty < 0 or ty > landblock_height) return;
            const int index = world::current_region->idx(tx,ty);

            if (!blocked) {
                if (tx >=0 and tx < landblock_width and ty>=0 and ty<landblock_height) {
                    view->last_visibility.push_back(index);
                    world::current_region->revealed[index] = true;
                }

                if (world::view_blocked[index] or
                        (world::current_region->tiles[index].base_tile_type == tile_type::RAMP and
                         level_band < world::current_region->tiles[index].level_band
                )) {
                    blocked = true;
                }
            }
        });
    }

}

void viewshed_system::scan_radius_penetrating(viewshed_component * view, const position_component * pos) {
    constexpr double sweep_degrees_radians = 0.5 * DEGRAD;
    constexpr double three_sixy_degrees_radians = 360 * DEGRAD;

    view->last_visibility.clear();

    // You can always see yourself
    const int x = pos->x;
    const int y = pos->y;
    const double radius = view->scanner_range;
    view->last_visibility.push_back(world::current_region->idx(x, y));

    // Sweep around
    for ( double angle=0; angle<three_sixy_degrees_radians; angle+=sweep_degrees_radians ) {
	double this_radius = radius;
	if (!is_facing_this_way(pos, angle) == true) this_radius = 2.0;
	
        pair<int,int> destination = project_angle ( x, y, this_radius, angle );
        line_func ( x, y, destination.first, destination.second, [view] (int tx, int ty) {
            const int index = world::current_region->idx(tx,ty);
            if (tx >=0 and tx < landblock_width and ty>=0 and ty<landblock_height) {
                view->last_visibility.push_back(index);
                world::current_region->revealed[index] = true;
            }
        });
    }
}

#pragma once

#include "base_system.h"
#include "../../game/world/world.h"
#include "ecs.h"
#include "position_component.h"
#include "viewshed_component.h"
#include "../../game/world/geometry.h"
#include <unordered_set>

using geometry::DEGRAD;
using geometry::line_func;
using geometry::project_angle;
using std::unordered_set;

namespace engine {
namespace ecs {

class viewshed_system : public base_system {
     virtual void tick ( const double &duration_ms ) {
	reset_visibility();
       
	// Run a viewshed for every system that has one
	const vector<viewshed_component *> viewsheds = find_components_by_type<viewshed_component> ( viewshed );
	for (viewshed_component * viewshed : viewsheds) {
	      const entity * parent = get_entity_by_handle ( viewshed->entity_id );
	      const int entity_handle = parent->find_component_by_type ( ecs::position );
	      position_component * pos = get_component_by_handle<position_component> ( entity_handle );
	      
	      if (pos->moved or viewshed->last_visibility.empty()) {
		switch (viewshed->scanner_type) {
		  case ecs::visibility : scan_radius_for_visibility(viewshed, pos); break;
		  case ecs::penetrating : scan_radius_penetrating(viewshed, pos); break;
		}
	      }
	      
	      // Apply the cached viewshed
	      for (const int &idx : viewshed->last_visibility) {
		  world::current_region->visible[idx] = true;
	      }
	}
     }
     
private:
    void reset_visibility() {
	std::fill(world::current_region->visible.begin(), world::current_region->visible.end(), false);
    }
    
    void scan_radius_for_visibility(viewshed_component * view, const position_component * pos) {
	scan_radius_penetrating(view,pos); // FIXME: This needs to be different once obstacles exist!
    }
    
    void scan_radius_penetrating(viewshed_component * view, const position_component * pos) {
	constexpr double sweep_degrees_radians = 1 * DEGRAD;
	constexpr double three_sixy_degrees_radians = 360 * DEGRAD;
	
	view->last_visibility.clear();
	
	unordered_set<int> visited(view->scanner_range * view->scanner_range);
	
	// You can always see yourself
	const int x = pos->x;
	const int y = pos->y;
	const int radius = view->scanner_range;
	view->last_visibility.push_back(world::current_region->idx(x, y));
	
	// Sweep around
	for ( double angle=0; angle<three_sixy_degrees_radians; angle+=sweep_degrees_radians ) {
	    pair<int,int> destination = project_angle ( x, y, radius, angle );
	    line_func ( x, y, destination.first, destination.second, [&visited,view] (int tx, int ty) {
		const int index = world::current_region->idx(tx,ty);
		auto finder = visited.find(index);
		if (finder==visited.end() and tx >=0 and tx <= landblock_width and ty>=0 and ty<=landblock_height) {
		    view->last_visibility.push_back(index);
		    world::current_region->revealed[index] = true;
		    visited.insert(index);
		}
	    });
	}
    }
};

}
}

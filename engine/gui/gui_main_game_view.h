#include "gui_element_base.h"
#include "../virtual_terminal.h"
#include "../../game/world/world.h"
#include <string>
#include <iostream>
#include <algorithm>
#include "../../engine/ecs/ecs.h"
#include "../ecs/position_component.h"

using std::string;
using engine::ecs::position_component;
using engine::ecs::entity;
using engine::ecs::get_entity_by_handle;
using engine::ecs::get_component_by_handle;
using engine::ecs::component_type;

namespace engine {
class gui_main_game_view : public gui_element_base {
public:
     gui_main_game_view ( const string name ) {
          tag = name;
     };

     virtual void render ( const screen_region viewport ) {

	  const entity * camera = get_entity_by_handle(world::camera_handle);
	  const position_component * camera_pos = get_component_by_handle<position_component>(camera->find_component_by_type(ecs::position));
	  
       
          const int left_x = std::max ( 0, camera_pos->x - viewport.w/2 );
          const int top_y = std::max ( 0, camera_pos->y - viewport.h/2 );
          const int right_x = std::min ( landblock_width-1, camera_pos->x + viewport.w/2 );
          const int bottom_y = std::min ( landblock_height-1, camera_pos->y + viewport.h/2 );

          int screen_y = viewport.y;
          for ( int y=top_y; y<=bottom_y; ++y ) {
               int screen_x = viewport.x;
               for ( int x=left_x; x<=right_x; ++x ) {
		    const int region_idx = world::current_region->idx ( x,y );
                    tile t = world::current_region->tiles[ region_idx ];
                    if ( world::current_region->revealed[ region_idx ] ) {
                         if ( world::current_region->visible[ region_idx ] ) {
			      auto finder = world::entity_render_list.find(region_idx);
			      if (finder == world::entity_render_list.end()) {
				  vterm::set_char_xy ( screen_x, screen_y, { t.display, t.foreground, t.background } );
			      } else {
				  vterm::set_char_xy ( screen_x, screen_y, finder->second );
			      }
                         } else {
                              vterm::set_char_xy ( screen_x, screen_y, { t.display, dark_grey, t.background } );
                         }
                    }
                    ++screen_x;
               }
               ++screen_y;
          }
	
     }

private:
};
}

#pragma once

#include <vector>
#include <utility>
#include "panel.h"
#include "../world/world.h"
#include "../../engine/sdl2_backend.h"
#include "line_buffer.h"
#include "colors.h"
#include "../game.h"

using std::vector;

namespace render {
  
struct stored_panel {
    int x, y;
    panel panel_info;
};
  
class panel_tile_info {
public:
  panel_tile_info ( sdl2_backend * sdl, const std::pair<int,int> region_loc, const std::pair<int,int> mouse_loc ) {
      SDL = sdl;
      int mouse_vx, mouse_vy;
      std::tie ( region_x, region_y ) = region_loc;
      std::tie ( mouse_vx, mouse_vy ) = mouse_loc;
      
      screen_x = mouse_vx*16 + 16;
      screen_y = mouse_vy*16 + 48;
      
      idx = world::current_region->idx( region_x, region_x );
    
      // We are going to make a panel for each major option group
      add_terrain_panel();
      add_structure_panels();
      add_settler_panels();
      add_build_options_panel();
  }
  
  void render() {}
  
private:
  sdl2_backend * SDL;
  vector<stored_panel> panels;
  int screen_x, screen_y, region_x, region_y;
  int idx;
  
  void create_panel_from_lines(line_buffer &lines) {
    const int height = lines.size()+16;
    const int width = lines.get_width()+32;
    panel p(SDL, height, width);
    SDL->texture_target( p.texture_id );
    const int x = 16;
    int y = 8;
    for ( const std::pair<std::string,SDL_Color> &panel_line : lines.lines ) {
      std::string line_s = SDL->render_text_to_image ( "disco14", panel_line.first, "tmp", panel_line.second );
      SDL->render_bitmap_simple ( line_s, x, y );
      y += 16;
    }
    SDL->reset_texture_target();
    
    stored_panel storage { 0, 0, p };
    panels.push_back( storage );
  }
  
  void add_terrain_panel() {
    line_buffer lines;
    lines.add_line( SDL, "Terrain Information", sdl_red );
    lines.add_line ( SDL, world::current_region->tiles[idx].get_description(), sdl_green );
    lines.add_line ( SDL, world::current_region->tiles[idx].get_climate(), sdl_cyan );
    lines.add_line( SDL, "Advice on digging will go here.", sdl_white );
    create_panel_from_lines( lines );
  }
  
  void add_structure_panels() {
    // Iterate components and find structures that are here
    line_buffer lines;
    
    vector<position_component *> positions = game_engine->ecs->find_components_by_func<position_component> (
     [this] ( const position_component &c ) {
          return ( c.x == this->region_x and c.y == this->region_y );
     });
    
    for (const position_component * pos : positions) {
	if ( game_engine->ecs->find_entity_component<settler_ai_component>( pos->entity_id ) == nullptr ) {
	    // It's not a settler, so we can describe it here
	    debug_name_component * name = game_engine->ecs->find_entity_component<debug_name_component>( pos->entity_id );
	    if (name != nullptr) {
		lines.add_line( SDL, name->debug_name, sdl_red );
		
		// Does it hold anything? If so - add that as well
		const int entity_id = pos->entity_id;
		vector<item_storage_component *> stored_items = game_engine->ecs->find_components_by_func<item_storage_component> (
		[entity_id] ( const item_storage_component &e ) {
		      if ( e.container_id == entity_id ) {
			  return true;
		      } else {
			  return false;
		      }
		} );
		for ( item_storage_component * item : stored_items ) {
		      debug_name_component * nc = game_engine->ecs->find_entity_component<debug_name_component> ( item->entity_id );
		      lines.add_line ( SDL, string ( " " ) + nc->debug_name, sdl_green );
		}
	    }
	}
    }
    
    create_panel_from_lines( lines );
  }
  
  void add_settler_panels() {
    // Iterate components and find settlers that are here
  }
  
  void add_build_options_panel() {
    // TODO: Build options!
  }
};
  
}
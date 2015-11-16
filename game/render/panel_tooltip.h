#pragma once

#include "panel.h"
#include "line_buffer.h"
#include <utility>
#include <algorithm>
#include <memory>
#include <sstream>
#include "../game.h"
#include "../world/world.h"
#include "../raws/raws.h"

using std::stringstream;

namespace render {

class panel_tooltip {
public:
     panel_tooltip ( sdl2_backend * sdl, const std::pair<int,int> region_loc, const std::pair<int,int> mouse_loc ) {
	  SDL = sdl;
          int region_x, region_y, mouse_vx, mouse_vy;
          std::tie ( region_x, region_y ) = region_loc;
          std::tie ( mouse_vx, mouse_vy ) = mouse_loc;

          const int idx = world::current_region->idx ( region_x, region_y );
	  if ( world::current_region->visible[ idx ] == false ) {
	      render_me = false;
	      return;
	  }

          screen_x = mouse_vx*16 + 16;
          screen_y = mouse_vy*16 + 48;

          lines.add_line ( SDL, "Right Click for Options", sdl_red );
	  lines.add_line ( SDL, world::current_region->tiles[idx].get_description(), sdl_green );
	  lines.add_line ( SDL, world::current_region->tiles[idx].get_climate(), sdl_cyan );
          add_tile_contents ( region_loc.first, region_loc.second );

	  render_buffer();
     }
     
     void render() {
	  SDL->set_alpha_mod ( "spritesheet", 128 );
	  SDL_Rect source { 0, 0, panel_holder->width, panel_holder->height };
	  int line_size = static_cast<int> ( (lines.size() /2 ) * 16 );
	  SDL_Rect dest { screen_x, screen_y - line_size, panel_holder->width, panel_holder->height };
	  SDL->render_bitmap( panel_holder->texture_id, source, dest );
	  SDL->set_alpha_mod ( "spritesheet", 255 );
     }
     
     void render_buffer() {
	  const int width = lines.get_width() + 32;
	  const int height = ( lines.size() * 16 ) + 8;
	  panel_holder = std::make_unique<panel>( SDL, width, height );
	  SDL->texture_target ( panel_holder->texture_id );
	  
	  SDL_Rect source = raws::get_tile_source_by_name ( "BLACKMASK" );
	  SDL_Rect dest { 0, 0, width, height };
	  SDL->render_bitmap ( "spritesheet", source, dest );
	  
	  int y = 8;
	  const int x = 16;
	  for ( const std::pair<std::string,SDL_Color> &tooltip_line : lines.lines ) {
               std::string line_s = SDL->render_text_to_image ( "disco14", tooltip_line.first, "tmp", tooltip_line.second );
               SDL->render_bitmap_simple ( line_s, x, y );
               y += 16;
          }
	  
	  SDL->reset_texture_target ();
     }

     void add_tile_contents ( const int region_x, const int region_y ) {
          vector<position_component *> positions = game_engine->ecs->find_components_by_func<position_component> (
          [region_x, region_y] ( const position_component &c ) {
               return ( c.x == region_x and c.y == region_y );
          }
                    );
          for ( const position_component * pos : positions ) {
               const int entity_id = pos->entity_id;
               if ( !add_settler_details ( entity_id ) ) add_name_details ( entity_id );
               add_containers ( entity_id );
          }
     }

     inline bool add_settler_details ( const int entity_id ) {
          const settler_ai_component * settler = game_engine->ecs->find_entity_component<settler_ai_component> ( entity_id );
          if ( settler == nullptr ) return false;

          stringstream settler_name;
          settler_name << settler->first_name << " " << settler->last_name << " (" << settler->profession_tag << ")";
          lines.add_line ( SDL, settler_name.str(), render::sdl_yellow );

          return true;
     }

     inline void add_name_details ( const int entity_id ) {
          const debug_name_component * name = game_engine->ecs->find_entity_component<debug_name_component> ( entity_id );
          if ( name != nullptr ) {
               lines.add_line ( SDL, name->debug_name, render::sdl_white );
          }
     }

     inline void add_containers ( const int entity_id ) {
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
               lines.add_line ( SDL, string ( " " ) + nc->debug_name, render::sdl_green );
          }
     }

     int screen_x;
     int screen_y;
     std::unique_ptr<panel> panel_holder;
     line_buffer lines;
     sdl2_backend * SDL;
     bool render_me = true;
};

}
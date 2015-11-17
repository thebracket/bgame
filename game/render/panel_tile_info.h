#pragma once

#include <vector>
#include <utility>
#include <sstream>
#include <memory>
#include "panel.h"
#include "../world/world.h"
#include "../../engine/sdl2_backend.h"
#include "line_buffer.h"
#include "colors.h"
#include "../game.h"
#include "../raws/raws.h"

using std::vector;
using std::stringstream;

namespace render {
  
struct stored_panel {
    int x, y;
    std::unique_ptr<panel> panel_info;
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
      add_structure_panels(); // Settler panels are called from here
      add_build_options_panel();
  }
  
  void render() {
    int y = 90;
    
    for (const stored_panel &p : panels) {
      
      SDL_Rect src { 0, 0, p.panel_info->width, p.panel_info->height };
      SDL_Rect dst { 100, y, p.panel_info->width, p.panel_info->height };
      SDL->render_bitmap( p.panel_info->texture_id, src, dst );
      
      y += p.panel_info->height + 10;
    }
  }
  
private:
  sdl2_backend * SDL;
  vector<stored_panel> panels;
  int screen_x, screen_y, region_x, region_y;
  int idx;
  
  void create_panel_from_lines(line_buffer &lines) {
    const int width = lines.get_width() + 32;
    const int height = ( lines.size() * 16 ) + 16;
    std::unique_ptr<panel> p = std::make_unique<panel>(SDL, width, height);
    
    SDL->texture_target( p->texture_id );
    SDL->clear_screen();
    
    SDL_Rect source = raws::get_tile_source_by_name ( "BLACKMASK" );
    SDL_Rect dest { 0, 0, width, height };
    SDL->render_bitmap ( "spritesheet", source, dest );
    
    const int x = 16;
    int y = 8;
    for ( const std::pair<std::string,SDL_Color> &panel_line : lines.lines ) {
      std::string line_s = SDL->render_text_to_image ( "disco14", panel_line.first, "tmp", panel_line.second );
      SDL->render_bitmap_simple ( line_s, x, y );
      y += 16;
    }
    SDL->reset_texture_target();
    
    panels.push_back( stored_panel{0,0,std::move(p)} );
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
	settler_ai_component * settler = game_engine->ecs->find_entity_component<settler_ai_component>( pos->entity_id );
	if ( settler == nullptr ) {
	    // It's not a settler, so we can describe it here
	    debug_name_component * name = game_engine->ecs->find_entity_component<debug_name_component>( pos->entity_id );
	    if (name != nullptr) {
		lines.add_line( SDL, name->debug_name, sdl_yellow );
		
		description_component * desc = game_engine->ecs->find_entity_component<description_component>( pos->entity_id );
		if (desc != nullptr) {
		  lines.add_line( SDL, desc->desc, sdl_white );
		}
		
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
		      desc = game_engine->ecs->find_entity_component<description_component>( item->entity_id );
		      if ( desc != nullptr ) {
			lines.add_line( SDL, string ("  ") + desc->desc, sdl_white );
		      }
		}
	    }
	    create_panel_from_lines( lines );
	} else {
	    add_settler_panel( pos, settler );
	}
    }
    
  }
  
  void add_settler_panel( const position_component * pos, const settler_ai_component * settler ) {
    line_buffer lines;
    game_health_component * health = game_engine->ecs->find_entity_component<game_health_component>( pos->entity_id );
    game_species_component * species = game_engine->ecs->find_entity_component<game_species_component>( pos->entity_id );
    game_stats_component * stats = game_engine->ecs->find_entity_component<game_stats_component>( pos->entity_id );
    
    {
    stringstream ss;
    ss << settler->first_name << " " << settler->last_name << " (" << settler->profession_tag << ")";
    lines.add_line ( SDL, ss.str(), sdl_yellow );
    ss.clear();
    }
    
    {
    stringstream ss;
    ss << "Hit points: " << health->current_hit_points << " / " << health->max_hit_points;
    lines.add_line( SDL, ss.str(), sdl_red );
    }
    
    switch (species->gender) {
      case gender_t::MALE : lines.add_line ( SDL, "Male", sdl_white ); break;
      case gender_t::FEMALE : lines.add_line ( SDL, "Female", sdl_white ); break;
      case gender_t::HERMAPHRODITE : lines.add_line ( SDL, "Hermaphrodite", sdl_white ); break;
      case gender_t::ASEXUAL : lines.add_line ( SDL, "Asexual", sdl_white ); break;
    }
    
    switch (species->sexual_preference) {
      case preference_t::HETEROSEXUAL : lines.add_line ( SDL, "Heterosexual", sdl_white ); break;
      case preference_t::HOMOSEXUAL : lines.add_line ( SDL, "Homosexual", sdl_white ); break;
      case preference_t::BISEXUAL : lines.add_line ( SDL, "Bisexual", sdl_white ); break;
      case preference_t::ASEXUAL : lines.add_line ( SDL, "Asexual", sdl_white ); break;
    }
    
    {
    stringstream ss;
    ss << "Strength: " << stats->strength;
    lines.add_line( SDL, ss.str(), sdl_green );
    }
    {
    stringstream ss;
    ss << "Dexterity: " << stats->dexterity;
    lines.add_line( SDL, ss.str(), sdl_green );
    }
    {
    stringstream ss;
    ss << "Constitution: " << stats->constitution;
    lines.add_line( SDL, ss.str(), sdl_green );
    }
    {
    stringstream ss;
    ss << "Intelligence: " << stats->intelligence;
    lines.add_line( SDL, ss.str(), sdl_green );
    }
    {
    stringstream ss;
    ss << "Wisdom: " << stats->wisdom;
    lines.add_line( SDL, ss.str(), sdl_green );
    }
    {
    stringstream ss;
    ss << "Charisma: " << stats->charisma;
    lines.add_line( SDL, ss.str(), sdl_green );
    }
    {
    stringstream ss;
    ss << "Comeliness: " << stats->comeliness;
    lines.add_line( SDL, ss.str(), sdl_green );
    }
        
    create_panel_from_lines( lines );
  }
  
  void add_build_options_panel() {
    line_buffer lines;
    
    lines.add_line( SDL, "Build options go here", sdl_white );
    
    create_panel_from_lines( lines );
  }
};
  
}
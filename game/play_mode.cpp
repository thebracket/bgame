#include "play_mode.h"
#include "world/world.h"
#include "systems/system_factory.h"
#include "../engine/gui/gui_frame.h"
#include "../engine/gui/gui_static_text.h"
#include "gui/gui_main_game_view.h"
#include "game.h"
#include "raws/raws.h"

using namespace engine;
using std::make_unique;

class game_render : public engine::base_node {
public:
    virtual void render(sdl2_backend * SDL) {

      render_map( SDL );
      // Render the power bar
      // Render date/time
      // Render paused
      // Render any emote pop-ups
      // Render any tool-tips
      
    }
    
    inline void set_base_source(SDL_Rect &source, const int &idx) {
	if (world::current_region->tiles[ idx ].base_tile_type == tile_type::WATER ) {
	    source = raws::get_tile_source_by_name("WATER"); // Water
	} else if ( world::current_region->tiles[ idx ].base_tile_type == tile_type::RAMP or
	  world::current_region->tiles[ idx ].base_tile_type > 4
	) {
	    switch (world::current_region->tiles[ idx ].base_tile_type) {
	      case tile_type::RAMP_NU_SD : source = raws::get_tile_source_by_name("RAMP_NU_SD"); break;
	      case tile_type::RAMP_ED_WU : source = raws::get_tile_source_by_name("RAMP_ED_WU"); break;
	      case tile_type::RAMP_EU_WD : source = raws::get_tile_source_by_name("RAMP_EU_WD"); break;
	      case tile_type::RAMP_ND_SU : source = raws::get_tile_source_by_name("RAMP_ND_SU"); break;
	      case tile_type::RAMP_EU_SU : source = raws::get_tile_source_by_name("RAMP_EU_SU"); break;
	      case tile_type::RAMP_WU_SU : source = raws::get_tile_source_by_name("RAMP_WU_SU"); break;
	      case tile_type::RAMP_EU_NU : source = raws::get_tile_source_by_name("RAMP_EU_NU"); break;
	      case tile_type::RAMP_WU_NU : source = raws::get_tile_source_by_name("RAMP_WU_NU"); break;
	      case tile_type::RAMP_WD_ND : source = raws::get_tile_source_by_name("RAMP_WD_ND"); break;
	      case tile_type::RAMP_ED_ND : source = raws::get_tile_source_by_name("RAMP_ED_ND"); break;
	      case tile_type::RAMP_WD_SD : source = raws::get_tile_source_by_name("RAMP_WD_SD"); break;
	      case tile_type::RAMP_ED_SD : source = raws::get_tile_source_by_name("RAMP_ED_SD"); break;
	      default: source = raws::get_tile_source_by_name("DOOD");
	    }
	} else {
	    switch (world::current_region->tiles [ idx ].ground ) {
	      case tile_ground::IGNEOUS : source = raws::get_tile_source_by_name("IGNEOUS"); break;
	      case tile_ground::SEDIMENTARY : raws::get_tile_source_by_name("SEDIMENTARY"); break;
	      case tile_ground::GRAVEL : source = raws::get_tile_source_by_name("GRAVEL"); break;
	      case tile_ground::WHITE_SAND : source = raws::get_tile_source_by_name("WHITE_SAND"); break;
	      case tile_ground::YELLOW_SAND : source = raws::get_tile_source_by_name("YELLOW_SAND"); break;
	      case tile_ground::RED_SAND : source = raws::get_tile_source_by_name("RED_SAND"); break;
	      default : std::cout << "Oops: unknown ground type : " << world::current_region->tiles [ idx ].ground << "\n";
	    }
	    source = raws::get_tile_source_by_name("SEDIMENTARY");
	}	
    }
    
    inline bool set_covering_source(SDL_Rect &source, const int &idx) {
	  bool render_cover = false;
	  if ( world::current_region->tiles[ idx ].base_tile_type == tile_type::RAMP or
	    world::current_region->tiles[ idx ].base_tile_type > 4
	  ) return false;
	  
	  if (world::current_region->tiles [ idx ].covering == tile_covering::CACTUS) { source = raws::get_tile_source_by_name("CACTUS"); render_cover = true; }
	  if (world::current_region->tiles [ idx ].covering == tile_covering::GORSE) { source = raws::get_tile_source_by_name("GORSE"); render_cover = true; }
	  if (world::current_region->tiles [ idx ].covering == tile_covering::GRASS) { source = raws::get_tile_source_by_name("GRASS"); render_cover = true; }
	  if (world::current_region->tiles [ idx ].covering == tile_covering::HEATHER) { source = raws::get_tile_source_by_name("HEATHER"); render_cover = true; }
	  if (world::current_region->tiles [ idx ].covering == tile_covering::LYCHEN) { source = raws::get_tile_source_by_name("LYCHEN"); render_cover = true; }
	  if (world::current_region->tiles [ idx ].covering == tile_covering::MOSS) { source = raws::get_tile_source_by_name("MOSS"); render_cover = true; }
	  if (world::current_region->tiles [ idx ].covering == tile_covering::REEDS) { source = raws::get_tile_source_by_name("REEDS"); render_cover = true; }
	  if (world::current_region->tiles [ idx ].covering == tile_covering::SHRUB) { source = raws::get_tile_source_by_name("SHRUB"); render_cover = true; }
	  if (world::current_region->tiles [ idx ].covering == tile_covering::THISTLE) { source = raws::get_tile_source_by_name("THISTLE"); render_cover = true; }
	  if (world::current_region->tiles [ idx ].covering == tile_covering::WILDFLOWER) { source = raws::get_tile_source_by_name("WILDFLOWER"); render_cover = true; }	  
	  return render_cover;
    }
    
    inline void render_map(sdl2_backend * SDL) {
      const position_component * camera_pos = game_engine->ecs->find_entity_component<position_component>(world::camera_handle);
      
      int region_y = camera_pos->y - 23;
      int left_x = camera_pos->x - 32;
      
      // map goes from 0,48 to 1024,768
      SDL_Rect source = {16,32,16,16};
      for (int y=0; y<45; ++y) {
	int region_x = left_x;
	for (int x=0; x<64; ++x) {
	    const int idx = world::current_region->idx(region_x, region_y);
 	    if (region_x >= 0 and region_x < landblock_width and region_y > 0 and region_y <= landblock_height-1 and world::current_region->revealed[idx] ) {
//	    if (region_x >= 0 and region_x < landblock_width and region_y > 0 and region_y <= landblock_height-1 ) {
		// Render the base ground
		set_base_source(source, idx);
		SDL_Rect dest = {x*16, (y*16) + 48, 16, 16};
		SDL->render_bitmap("spritesheet", source, dest);
		
		// Render any covering
		bool render_cover = set_covering_source(source, idx);
		if (render_cover) {
		  SDL->render_bitmap("spritesheet", source, dest);
		}
		
		// Render any renderable items for this tile
		auto finder = world::entity_render_list.find( idx );
		if ( finder != world::entity_render_list.end() ) {
		    const int sprite_idx = finder->second;
		    source = raws::get_tile_source( sprite_idx );
		    SDL->render_bitmap("spritesheet", source, dest);
		}
		
		// Render darkness
		bool render_darkness = true;
		const float terrain_angle = world::current_region->tiles[idx].surface_normal;
		const float angle_difference = std::abs(terrain_angle - world::sun_angle);
		float intensity_pct = 1.0 - (std::abs(angle_difference)/90.0F);
		if (intensity_pct < 0.25) intensity_pct = 0.25;
		if (intensity_pct > 1.0) intensity_pct = 1.0;
		if (world::current_region->tiles[idx].base_tile_type == tile_type::WATER) intensity_pct = 1.0;
		// TODO: Use this to select an appropriate mask.
		if (intensity_pct <= 0.25) { source = raws::get_tile_source_by_name("DARK75"); }
		else if (intensity_pct <= 0.5) { source = raws::get_tile_source_by_name("DARK50"); }
		else if (intensity_pct <= 0.75) { source = raws::get_tile_source_by_name("DARK25"); }
		else { render_darkness = false; }
		if (render_darkness) SDL->render_bitmap("spritesheet", source, dest);
		
		// Render not visible
		if (world::current_region->visible[idx] == false) {
		    source = raws::get_tile_source_by_name("HIDEMASK");
		    SDL->render_bitmap("spritesheet", source, dest);
		}
	    }
	    ++region_x;
	}
	++region_y;
      }
    }
};

void play_mode::init_systems()
{
     game_engine->ecs->add_system ( make_input_system() );
     game_engine->ecs->add_system ( make_camera_system() );
     game_engine->ecs->add_system ( make_calendar_system() );
     game_engine->ecs->add_system ( make_obstruction_system() );
     game_engine->ecs->add_system ( make_flowmap_system() );
     game_engine->ecs->add_system ( make_power_system() );
     game_engine->ecs->add_system ( make_settler_ai_system() );
     game_engine->ecs->add_system ( make_viewshed_system() );
     game_engine->ecs->add_system ( make_renderable_system() );
     
     SDL_Rect all {0, 0, 1024, 48};
     sg.children.push_back( make_unique<scene_blit>( "header", all, all ) );
     sg.children.push_back( make_unique<game_render>() );
}

void play_mode::init()
{
     game_engine->ecs->init();
     game_engine->ecs->load_game("world/saved_game.dat");
     init_systems();
     
     quitting = false;
     world::log.write ( "Welcome to @B_YELLOW@Black Future" );
     world::log.write ( "Use the @B_WHITE@arrow keys@WHITE@ to move, or press @B_RED@Q@WHITE@ to quit." );
     int widx = world::world_idx(2,2);
     world::current_region = new land_block(widx);
}

void play_mode::done()
{
    world::current_region->save();
    game_engine->ecs->save_game("world/saved_game.dat");
    delete world::current_region;
    game_engine->ecs->done();
}

pair< return_mode, unique_ptr< base_mode > > play_mode::tick ( const double time_elapsed )
{
     if ( command::is_key_down ( command::Q ) ) quitting = true;

     if ( quitting ) {
	  return make_pair ( POP, NO_PUSHED_MODE );
     }
     return make_pair ( CONTINUE, NO_PUSHED_MODE );
}

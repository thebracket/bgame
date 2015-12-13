#include "game_mode.hpp"
#include "../engine/command_manager.h"
#include <utility>
#include "world/planet.hpp"
#include "world/world_defs.hpp"
#include "../engine/virtual_terminal.h"
#include <sstream>
#include "../game/game.h"
#include "world/universe.hpp"
#include "systems/system_factory.h"

using std::make_pair;
using namespace engine;
using namespace engine::command;
using engine::vterm::color_t;
using std::make_unique;

class game3d_render : public engine::base_node
{
public:
    inline void render_ascii(const SDL_Rect &dest, const vterm::screen_character &target, sdl2_backend * SDL, int depth=0) {
	unsigned char target_char = target.character;
	int texture_x = ( target_char % 16 ) * 8;
	int texture_y = ( target_char / 16 ) * 8;
	SDL_Rect source = {texture_x, texture_y, 8, 8 };
	
	color_t fg = target.foreground_color;
	if (depth > 0) {
	    const int darken = depth*10;
	    if (std::get<0>(fg) > darken) std::get<0>(fg) = ( std::get<0>(fg)-darken );
	    if (std::get<1>(fg) > darken) std::get<1>(fg) = ( std::get<1>(fg)-darken );
	    if (std::get<2>(fg) > darken) std::get<2>(fg) = ( std::get<2>(fg)-darken );
	}
	
	SDL->set_color_mod( "font_s", std::get<0>(fg), std::get<1>(fg), std::get<2>(fg) );
	SDL->render_bitmap( "font_s", source, dest);
    }
  
    virtual void render ( sdl2_backend * SDL ) override {
	position_component3d * camera_pos = game_engine->ecs->find_entity_component<position_component3d>( world::camera_handle );
	pair<int,int> screen_size = SDL->get_screen_size();
	const int ascii_width = screen_size.first / 8;
	const int ascii_height = screen_size.first / 8;
	SDL_Rect viewport { camera_pos->pos.x - (ascii_width/2), camera_pos->pos.y - (ascii_height/2), ascii_width, ascii_height };
	if (camera_pos->pos.x < 0) camera_pos->pos.x = 0;
	if (camera_pos->pos.y < 0) camera_pos->pos.y = 0;
	
	for (int y = 0; y<viewport.h; ++y) {
	    for (int x = 0; x<viewport.w; ++x) {
		SDL_Rect dest{x*8,y*8,8,8};
		vterm::screen_character target;
		const location_t world_loc{ camera_pos->pos.region, viewport.x + x, viewport.y + y, camera_pos->pos.z };
		tile_t * tile = world::planet->get_tile( world_loc );
		if (!tile->solid and tile->base_tile_type==tile_type::EMPTY_SPACE) {
		    // 3d dive
		    int depth = 1;
		    bool go = false;
		    while (!go) {
			tile_t * dive_tile = world::planet->get_tile( location_t{ camera_pos->pos.region, viewport.x + x, viewport.y + y, camera_pos->pos.z-depth } );
			if (!dive_tile->solid and dive_tile->base_tile_type==tile_type::EMPTY_SPACE) {
			  ++depth;
			  if (depth > 10) go = true;
			} else {
			  if (dive_tile->solid) {
			    target.character = 219;
			    target.foreground_color = color_t{128,128,128};
			    target.background_color = color_t{0,0,0};
			  } else {
			    if (dive_tile->water_level > 0) {
			      target.character = 126;
			      target.foreground_color = color_t{0,0,128};
			      target.background_color = color_t{0,0,0};
			    } else {
			      target.character = 177;
			      target.foreground_color = color_t{0,128,0};
			      target.background_color = color_t{0,0,0};
			    }
			    go = true;
			}
		      }
		    }
		    //std::cout << "Dive reached depth: " << depth << "\n";
		    if (depth < 10) render_ascii(dest, target, SDL, depth);
		} else {
		    if (tile->solid) {
		      target.character = 219;
		      target.foreground_color = color_t{128,128,128};
		      target.background_color = color_t{0,0,0};
		    } else {
		      if (tile->water_level > 0) {
			target.character = 126;
			target.foreground_color = color_t{0,0,128};
			target.background_color = color_t{0,0,0};
		      } else {
			target.character = 177;
			target.foreground_color = color_t{0,128,0};
			target.background_color = color_t{0,0,0};
		      }
		      
		    }
		    render_ascii(dest, target, SDL, 0);

		}
	    }
	}
    }
private:

};

void game_mode::init_systems()
{
     game_engine->ecs->add_system ( make_input_system() );
     game_engine->ecs->add_system ( make_camera_system() );
     //game_engine->ecs->add_system ( make_calendar_system() );
}

void game_mode::init()
{
     finished = false;
     world::planet->load_region ( world::planet->planet_idx ( WORLD_WIDTH/2, WORLD_HEIGHT-2 ) );
     
     SDL_Rect all {0, 0, 1024, 48};
     sg.children.push_back( make_unique<game3d_render>() );
     sg.children.push_back ( make_unique<scene_blit> ( "header", all, all ) );
     
     game_engine->ecs->init();
     game_engine->ecs->load_game( "world/savegame3d.dat" );
     init_systems();
}

void game_mode::done()
{

}

pair< engine::return_mode, unique_ptr< engine::base_mode > > game_mode::tick ( const double time_elapsed )
{
      // Temporary
     if ( is_key_down ( Q ) ) finished = true;

     if ( finished ) {
          return make_pair ( POP, NO_PUSHED_MODE );
     } else {
          return make_pair ( CONTINUE, NO_PUSHED_MODE );
     }
}

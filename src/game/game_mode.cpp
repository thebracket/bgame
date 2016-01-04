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
#include "render/colors.h"
#include <iomanip>
#include "render/panel_tooltip.h"

using std::make_pair;
using namespace engine;
using namespace engine::command;
using engine::vterm::color_t;
using std::make_unique;
using engine::vterm::desaturate;
using engine::vterm::darken;

enum game_mode_t { normal, radial };

class game3d_render : public engine::base_node {
public:
     int last_mouse_x = 0;
     int last_mouse_y = 0;
     int mouse_hover_time = 0;
  
     inline void render_ascii ( const SDL_Rect &dest, const vterm::screen_character &target, sdl2_backend * SDL, int depth=0, bool visible=true ) {
          unsigned char target_char = target.character;
          int texture_x = ( target_char % 16 ) * 8;
          int texture_y = ( target_char / 16 ) * 8;
          SDL_Rect source = {texture_x, texture_y, 8, 8 };
          //const SDL_Rect bg_source = { 88, 104, 8, 8 };

          color_t fg = target.foreground_color;
          if ( depth > 0 ) {
               const int darken_amount = depth*25;
               darken ( darken_amount, fg );
          }
          
          if (!visible and !world::omniscience) {
	      desaturate ( fg );
          }

          SDL->set_color_mod ( "font_s", std::get<0> ( fg ), std::get<1> ( fg ), std::get<2> ( fg ) );
          SDL->render_bitmap ( "font_s", source, dest );
     }

     virtual void render ( sdl2_backend * SDL ) override {
          render_power_bar ( SDL );
          render_date_time ( SDL );
          render_paused ( SDL );

          position_component3d * camera_pos = game_engine->ecs->find_entity_component<position_component3d> ( world::camera_handle );
	  region_t * current_region = world::planet->get_region( camera_pos->pos.region );
          pair<int,int> screen_size = SDL->get_screen_size();
          screen_size.second -= 48;
          const int ascii_width = screen_size.first / 8;
          const int ascii_height = screen_size.second / 8;
          if ( camera_pos->pos.x < ascii_width/2 ) camera_pos->pos.x = ascii_width/2;
          if ( camera_pos->pos.x > REGION_WIDTH - ( ascii_width/2 ) ) camera_pos->pos.x = REGION_WIDTH - ( ascii_width/2 );
          if ( camera_pos->pos.y < ascii_height/2 ) camera_pos->pos.y = ascii_height/2;
          if ( camera_pos->pos.y > REGION_HEIGHT- ( ascii_height/2 ) ) camera_pos->pos.y = REGION_HEIGHT - ( ascii_height/2 );
          SDL_Rect viewport { camera_pos->pos.x - ( ascii_width/2 ), camera_pos->pos.y - ( ascii_height/2 ), ascii_width, ascii_height };

          for ( int y = 0; y<viewport.h; ++y ) {
               for ( int x = 0; x<viewport.w; ++x ) {
                    SDL_Rect dest {x*8, ( y*8 ) +48,8,8};
                    vterm::screen_character target { '.', color_t{255,255,255}, color_t{0,0,0} };
                    const location_t world_loc {
                         camera_pos->pos.region, static_cast<uint8_t> ( viewport.x + x ), static_cast<uint8_t> ( viewport.y + y ), camera_pos->pos.z
                    };
                    tile_t * tile = world::planet->get_tile ( world_loc );
		    
		    const int tile_idx = get_tile_index( world_loc.x, world_loc.y, world_loc.z );
		    
                    if ( !tile->flags.test ( TILE_OPTIONS::SOLID ) and tile->base_tile_type==tile_type::EMPTY_SPACE and !world::render_list_3d[tile_idx] ) {
                         // 3d dive
                         int depth = 1;
                         bool go = false;
			 int dive_tile_idx;
                         while ( !go ) {
                              tile_t * dive_tile = world::planet->get_tile ( location_t { camera_pos->pos.region, world_loc.x, world_loc.y, static_cast<uint8_t> ( camera_pos->pos.z-depth ) } );
			      dive_tile_idx = get_tile_index( world_loc.x, world_loc.y, world_loc.z - depth );
			      
                              if ( !dive_tile->flags.test ( TILE_OPTIONS::SOLID ) and dive_tile->base_tile_type==tile_type::EMPTY_SPACE and !world::render_list_3d[dive_tile_idx] ) {
                                   ++depth;
                                   if ( depth > 10 ) go = true;
                              } else {
                                   if ( dive_tile->flags.test ( TILE_OPTIONS::SOLID ) ) {
                                        target.character = 219;
                                        target.foreground_color = color_t {128,128,128};
                                        target.background_color = color_t {0,0,0};
                                   } else {
                                        target = dive_tile->render_as;
                                        go = true;
                                   }

                                   if ( world::render_list_3d[dive_tile_idx] ) {
                                        target = world::render_list_3d[dive_tile_idx].value();
                                   }
                              }
                         }
                         //std::cout << "Dive reached depth: " << depth << "\n";
                         if ( depth < 10 and (current_region->revealed[dive_tile_idx] or world::omniscience )) {
			    render_ascii ( dest, target, SDL, depth, current_region->visible[dive_tile_idx] );
			 }
			 
                    } else {
                         if ( tile->flags.test ( TILE_OPTIONS::SOLID ) ) {
                              target.character = 219;
                              target.foreground_color = color_t {128,128,128};
                              target.background_color = color_t {0,0,0};
                         } else {
                              target = tile->render_as;
                         }
                         
                         if ( world::render_list_3d[tile_idx] ) {
                              target = world::render_list_3d[tile_idx].value();
                         }
                         if ( current_region->revealed[tile_idx] or world::omniscience ) {
			    render_ascii ( dest, target, SDL, 0, current_region->visible[tile_idx] );
			 }

                    }
               } // x
          } // y
          
	  render_emotes ( SDL, viewport );
	  render_particles ( SDL, viewport );
	  if ( mode == normal ) {
	    render_cursor ( SDL, screen_size, viewport, camera_pos );
	  } else if ( mode == radial ) {
          if ( !world::paused ) {
               mode = normal;
          } else {
	      // TODO: Render Radial
          }
     }
     }
private:
     game_mode_t mode = normal;
  
     void render_power_bar ( sdl2_backend * SDL ) {
          const float power_percent = static_cast<float> ( world::stored_power ) / static_cast<float> ( world::max_power );
          const int power_tenths = ( power_percent * 10.0 )-1;
          SDL_Rect src { power_tenths * 46, 0, 46, 48 };
          SDL_Rect dest { 4, 0, 48, 48 };
          SDL->render_bitmap ( "cordex", src, dest );

          std::stringstream ss;
          ss << "Power: " << world::stored_power << " I " << world::max_power;
          string emote_text = SDL->render_text_to_image ( "lcd10", ss.str(), "tmp", render::sdl_dark_grey );
          SDL->render_bitmap_simple ( emote_text, 68, 22 );
     }

     void render_date_time ( sdl2_backend * SDL ) {
          //SDL_Color sdl_white = {255,255,255,255};

          if ( world::display_day_month.empty() ) {
               world::display_day_month = " ";
          }
          if ( world::display_time.empty() ) {
               world::display_time = " ";
          }
          const std::string the_date = game_engine->render_text_to_image ( "lcd10", world::display_day_month, "btn_playgame", render::sdl_dark_grey );
          const std::string the_time = game_engine->render_text_to_image ( "lcd10", world::display_time, "btn_playgame", render::sdl_dark_grey );
          SDL->render_bitmap_simple ( the_date, 68, 8 );
          SDL->render_bitmap_simple ( the_time, 178, 8 );
     }

     void render_paused ( sdl2_backend * SDL ) {
          if ( world::paused ) {
               const std::string paused = game_engine->render_text_to_image ( "lcd10", "PAUSED", "btn_playgame", render::sdl_dark_grey );
               SDL->render_bitmap_simple ( paused, 310, 8 );
          }
     }
     
     void render_emotes ( sdl2_backend * SDL, SDL_Rect &viewport ) {
	  
	  vector<chat_emote_message> * emote_ptr = game_engine->messaging->get_messages_by_type<chat_emote_message>();
	  for ( chat_emote_message &emote : *emote_ptr) {
	      //std::cout << "Emote: " << emote.message << "\n";
	      const unsigned char fade = 4 * emote.ttl;
               const SDL_Color sdl_black {
                    0,0,0,0
               };
	       SDL_Color text_color = sdl_black;
	       switch ( emote.color ) {
		 case BLACK : text_color = render::sdl_black; break;
		 case WHITE : text_color =  render::sdl_white; break;
		 case YELLOW : text_color = render::sdl_yellow; break;
		 case CYAN : text_color = render::sdl_cyan; break;
		 case GREEN : text_color = render::sdl_green; break;
		 case MAGENTA : text_color = render::sdl_magenta; break;
		 case RED : text_color = render::sdl_red; break;
		 case BLUE : text_color = render::sdl_blue; break;
	       }
               string emote_text = SDL->render_text_to_image ( "disco12", emote.message, "tmp", sdl_black );
               SDL->set_alpha_mod ( emote_text, fade );
               SDL->set_alpha_mod ( "emote_bubble", fade );
               std::pair<int,int> emote_size = SDL->query_bitmap_size ( emote_text );

               // Left part of bubble
               const int x = ( emote.x - viewport.x ) * 8;
               const int y = ( emote.y - viewport.y ) * 8 + 48;
               const int height = emote_size.second;

               SDL_Rect src { 0, 0, 4, 8 };
               SDL_Rect dest { x , y , 4, height };
               SDL->render_bitmap ( "emote_bubble", src, dest );

               // Center of bubble
               src = { 5, 0, 4, 8 };
               dest = { x + 4, y, emote_size.first, height };
               SDL->render_bitmap ( "emote_bubble", src, dest );

               // Right part of bubble
               src = { 27, 0, 4, 8 };
               dest = { x + 4+emote_size.first, y, 4, height };
               SDL->render_bitmap ( "emote_bubble", src, dest );

               // The text itself
               SDL->render_bitmap_simple ( emote_text, x+4, y );
               SDL->set_alpha_mod ( "emote_bubble", 0 );
	  }
	  
	  vector<highlight_message> * highlights = game_engine->messaging->get_messages_by_type<highlight_message>();
	  for ( highlight_message &highlight : *highlights) {
	      SDL_Rect dest { (highlight.tile_x - viewport.x)*8, ((highlight.tile_y-viewport.y)*8)+48, 8, 8 };
	      engine::vterm::screen_character highlight_c { 219, color_t{ 255, 0, 255 }, color_t{ 0, 0, 0 } };
	      SDL->set_alpha_mod( "font_s", 128 + highlight.ttl );
	      render_ascii( dest, highlight_c, SDL );
	  }
	  SDL->set_alpha_mod( "font_s", 255 );
     }
     
     void render_particles ( sdl2_backend * SDL, SDL_Rect &viewport ) {	  
	  vector<particle_message> * particles = game_engine->messaging->get_messages_by_type<particle_message>();
	  for (particle_message &particle : *particles) {
	      //std::cout << "Particle!\n";
	      SDL_Color text_color = render::sdl_black;
	       switch ( particle.color ) {
		 case WHITE : text_color =  render::sdl_light_grey; break;
		 case YELLOW : text_color = render::sdl_yellow; break;
		 case CYAN : text_color = render::sdl_cyan; break;
		 case GREEN : text_color = render::sdl_green; break;
		 case MAGENTA : text_color = render::sdl_magenta; break;
		 case RED : text_color = render::sdl_red; break;
		 case BLUE : text_color = render::sdl_blue; break;
		 case BLACK : text_color = render::sdl_dark_grey; break;
	       }
	      string emote_text = SDL->render_text_to_image ( "disco12", particle.message, "tmp", text_color );
	      std::pair<int,int> emote_size = SDL->query_bitmap_size ( emote_text );
	      const int x = ( particle.tile_x - viewport.x ) * 8;
	      const int y = ( particle.tile_y - viewport.y -1 ) * 8 + 48 + 4;
	      SDL_Rect src{ 0, 0, emote_size.first, emote_size.second};
	      SDL_Rect dst{ x+particle.offset_x, y+particle.offset_y, particle.ttl/4, particle.ttl/4 };
	      SDL->render_bitmap( emote_text, src, dst );
	  }
     }
     
     std::unique_ptr<render::panel_tooltip> tooltip_window;
     
     void render_cursor ( sdl2_backend * SDL, pair<int,int> &screen_size, SDL_Rect &viewport, position_component3d * camera_pos ) {
	int mouse_x = engine::command::mouse_x;
	int mouse_y = engine::command::mouse_y;
	
	if (mouse_x > 0 and mouse_x < screen_size.first and mouse_y > 48 and mouse_y < screen_size.second) {
	    const int tile_x = mouse_x/8;
	    const int tile_y = (mouse_y-48)/8;
	    	    
	    const int16_t tilespace_x = tile_x + viewport.x;
	    const int16_t tilespace_y = tile_y + viewport.y;
	    
	    if (tilespace_x == last_mouse_x and tilespace_y == last_mouse_y) {
		++mouse_hover_time;
	    } else {
		mouse_hover_time = 0;		
		if ( tooltip_window ) tooltip_window.reset();
	    }
	    last_mouse_x = tilespace_x;
	    last_mouse_y = tilespace_y;
	    
	    const location_t target { camera_pos->pos.region, tilespace_x, tilespace_y, camera_pos->pos.z };
	    const int target_idx = get_tile_index( tilespace_x, tilespace_y, camera_pos->pos.z );
	    
	    engine::vterm::screen_character cursor { 9, color_t{255,255,0}, color_t{0,0,0} };
	    if ( !world::planet->get_region( target.region )->revealed [ target_idx ] ) {
		cursor.foreground_color = color_t { 64, 64, 64 };
	    } else {
		tile_t * target_tile = world::planet->get_tile( target );
		if (target_tile->flags.test ( TILE_OPTIONS::SOLID ) ) {
		    cursor.foreground_color = color_t { 255, 0, 0 };
		}
		if (target_tile->flags.test ( TILE_OPTIONS::CAN_STAND_HERE ) ) {
		    cursor.foreground_color = color_t { 0, 255, 255 };
		}
	    }
	    
	    int alpha = 32 + (mouse_hover_time*8);
	    if (alpha > 200) alpha = 200;
	    SDL->set_alpha_mod( "font_s", alpha );
	    SDL_Rect dest { tile_x * 8, (tile_y * 8)+48, 8, 8 };
	    render_ascii( dest, cursor, SDL );
	    SDL->set_alpha_mod( "font_s", 255 );
	    
	    if ( mouse_hover_time > 10 and world::planet->get_region( camera_pos->pos.region )->revealed [ target_idx ] ) {
		render_tooltip( SDL, target, std::make_pair( tilespace_x - viewport.x, tilespace_y - viewport.y ) );
	    }
	}
     }

     void render_tooltip ( sdl2_backend * SDL, const location_t &loc, const std::pair<int,int> mouse ) {
	  if ( tooltip_window ) {
	      tooltip_window->render( mouse_hover_time );
	      return;
	  }
       	  	  
	  tooltip_window = std::make_unique<render::panel_tooltip>( SDL, loc, mouse );
	  tooltip_window->render( mouse_hover_time );
     }
};

void game_mode::init_systems()
{
     game_engine->ecs->add_system ( make_input_system() );
     game_engine->ecs->add_system ( make_camera_system() );
     game_engine->ecs->add_system ( make_calendar_system() );
     game_engine->ecs->add_system ( make_obstruction_system() );
     game_engine->ecs->add_system ( make_inventory_system() );
     game_engine->ecs->add_system ( make_power_system() );
     game_engine->ecs->add_system ( make_cordex_ai_system() );
     game_engine->ecs->add_system ( make_settler_ai_system() );
     game_engine->ecs->add_system ( make_damage_system() );
     game_engine->ecs->add_system ( make_particle_system() );
     game_engine->ecs->add_system ( make_viewshed_system() );
     game_engine->ecs->add_system ( make_renderable_system() );
}

void game_mode::init()
{
     finished = false;
     world::planet->load_region ( world::planet->planet_idx ( WORLD_WIDTH/2, WORLD_HEIGHT-1 ) );

     SDL_Rect all {0, 0, 1024, 48};
     sg.children.push_back ( make_unique<scene_blit> ( "header", all, all ) );
     sg.children.push_back ( make_unique<game3d_render>() );

     game_engine->ecs->init();
     game_engine->ecs->load_game ( "world/savegame3d.dat" );
     init_systems();

     // Tell the rendering and movement systems to fire
     game_engine->messaging->add_message<walkability_changed_message> ( walkability_changed_message() );
     game_engine->messaging->add_message<entity_moved_message> ( entity_moved_message() );
}

void game_mode::done()
{
    game_engine->ecs->save_game( "world/savegame3d.dat" );
    game_engine->ecs->done();
}

pair< engine::return_mode, unique_ptr< engine::base_mode > > game_mode::tick ( const double time_elapsed )
{
     // Temporary
     if ( is_key_down ( Q ) ) finished = true;
     if ( is_key_down( command::TILDE ) ) {
       vector<tuple<string,int,int,int>> timings = game_engine->ecs->get_profile_info();
       for ( auto line : timings ) {
	 std::cout << std::setw(20) << std::get<0>(line) << std::setw(10) << std::get<1>(line)<< std::setw(10) << std::get<2>(line)<< std::setw(10) << std::get<3>(line) << "\n";
       }
     }
     if ( is_key_down( F10) ) {
	world::omniscience = !world::omniscience;
     }

     if ( finished ) {
          return make_pair ( POP, NO_PUSHED_MODE );
     } else {
          return make_pair ( CONTINUE, NO_PUSHED_MODE );
     }
}

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

using std::make_pair;
using namespace engine;
using namespace engine::command;
using engine::vterm::color_t;
using std::make_unique;

inline void darken ( const int &amount, color_t &col )
{
     unsigned char red = std::get<0> ( col );
     unsigned char green = std::get<1> ( col );
     unsigned char blue = std::get<2> ( col );

     if ( red > amount ) {
          red -= amount;
     } else {
          red = 0;
     }
     if ( green > amount ) {
          green -= amount;
     } else {
          green = 0;
     }
     if ( blue > amount ) {
          blue -= amount;
     } else {
          blue = 0;
     }

     std::get<0> ( col ) = red;
     std::get<1> ( col ) = green;
     std::get<2> ( col ) = blue;
}

class game3d_render : public engine::base_node {
public:
     inline void render_ascii ( const SDL_Rect &dest, const vterm::screen_character &target, sdl2_backend * SDL, int depth=0 ) {
          unsigned char target_char = target.character;
          int texture_x = ( target_char % 16 ) * 8;
          int texture_y = ( target_char / 16 ) * 8;
          SDL_Rect source = {texture_x, texture_y, 8, 8 };
          const SDL_Rect bg_source = { 88, 104, 8, 8 };

          color_t fg = target.foreground_color;
          if ( depth > 0 ) {
               const int darken_amount = depth*25;
               darken ( darken_amount, fg );
          }

          SDL->set_color_mod ( "font_s", std::get<0> ( fg ), std::get<1> ( fg ), std::get<2> ( fg ) );
          SDL->render_bitmap ( "font_s", source, dest );
     }

     virtual void render ( sdl2_backend * SDL ) override {
          render_power_bar ( SDL );
          render_date_time ( SDL );
          render_paused ( SDL );

          position_component3d * camera_pos = game_engine->ecs->find_entity_component<position_component3d> ( world::camera_handle );
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
                    vterm::screen_character target;
                    const location_t world_loc {
                         camera_pos->pos.region, static_cast<uint8_t> ( viewport.x + x ), static_cast<uint8_t> ( viewport.y + y ), camera_pos->pos.z
                    };
                    tile_t * tile = world::planet->get_tile ( world_loc );
		    
		    const int tile_idx = ( camera_pos->pos.z * REGION_HEIGHT*REGION_WIDTH ) + ( ( viewport.y + y ) * REGION_WIDTH ) + ( viewport.x + x );
                    if ( !tile->solid and tile->base_tile_type==tile_type::EMPTY_SPACE and !world::render_list_3d[tile_idx] ) {
                         // 3d dive
                         int depth = 1;
                         bool go = false;
                         while ( !go ) {
                              tile_t * dive_tile = world::planet->get_tile ( location_t { camera_pos->pos.region, world_loc.x, world_loc.y, static_cast<uint8_t> ( camera_pos->pos.z-depth ) } );
                              const int dive_tile_idx = ( ( camera_pos->pos.z-depth ) * REGION_HEIGHT*REGION_WIDTH ) + ( ( viewport.y + y ) * REGION_WIDTH ) + ( viewport.x + x );

                              if ( !dive_tile->solid and dive_tile->base_tile_type==tile_type::EMPTY_SPACE and !world::render_list_3d[dive_tile_idx] ) {
                                   ++depth;
                                   if ( depth > 10 ) go = true;
                              } else {
                                   if ( dive_tile->solid ) {
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
                         if ( depth < 10 ) render_ascii ( dest, target, SDL, depth );
			 
                    } else {
                         if ( tile->solid ) {
                              target.character = 219;
                              target.foreground_color = color_t {128,128,128};
                              target.background_color = color_t {0,0,0};
                         } else {
                              target = tile->render_as;
                         }
                         //const int tile_idx = ( camera_pos->pos.z * REGION_HEIGHT*REGION_WIDTH ) + ( ( viewport.y + y ) * REGION_WIDTH ) + ( viewport.x + x );
                         if ( world::render_list_3d[tile_idx] ) {
                              target = world::render_list_3d[tile_idx].value();
                              //std::cout << "Hit!\n";
                         }
                         render_ascii ( dest, target, SDL, 0 );

                    }
               }
          }
     }
private:
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
          SDL_Color sdl_white = {255,255,255,255};

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

};

void game_mode::init_systems()
{
     game_engine->ecs->add_system ( make_input_system() );
     game_engine->ecs->add_system ( make_camera_system() );
     game_engine->ecs->add_system ( make_calendar_system() );
     //game_engine->ecs->add_system ( make_obstruction_system() );
     //game_engine->ecs->add_system ( make_inventory_system() );
     game_engine->ecs->add_system ( make_power_system() );
     //game_engine->ecs->add_system ( make_cordex_ai_system() );
     //game_engine->ecs->add_system ( make_settler_ai_system() );
     //game_engine->ecs->add_system ( make_damage_system() );
     //game_engine->ecs->add_system ( make_particle_system() );
     //game_engine->ecs->add_system ( make_viewshed_system() );
     game_engine->ecs->add_system ( make_renderable_system() );
}

void game_mode::init()
{
     finished = false;
     world::planet->load_region ( world::planet->planet_idx ( WORLD_WIDTH/2, WORLD_HEIGHT-2 ) );

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

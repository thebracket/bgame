#include "genesis_mode.h"
#include "world/universegen.hpp"
#include "../game/game.h"
#include "../engine/geometry.hpp"

using std::make_pair;
using namespace engine;
using namespace engine::command;

void genesis_mode::init()
{
     mode = INIT;
}

void genesis_mode::done()
{
}

pair< engine::return_mode, unique_ptr< engine::base_mode > > genesis_mode::tick ( const double time_elapsed )
{
     switch ( mode ) {
     case INIT :
          begin_genesis ( time_elapsed );
          break;
     case UGEN :
          ugen ( time_elapsed );
          break;
     case UDISPLAY :
          udisplay ( time_elapsed );
          break;
     case UDISPLAY2:
          udisplay2 ( time_elapsed );
          break;
     case TRAVEL :
          travel ( time_elapsed );
          break;
     case SYSTEM_SCAN :
          system_scan ( time_elapsed );
          break;
     }



     // Temporary
     if ( is_key_down ( ENTER ) ) finished = true;

     if ( finished ) {
          return make_pair ( POP, NO_PUSHED_MODE );
     } else {
          return make_pair ( CONTINUE, NO_PUSHED_MODE );
     }
}

void genesis_mode::intro_quote()
{
     const string msg1 {"Space is big. Really big. You just won't"
     };
     const string msg2 {"believe how vastly, hugely, mind-bogglingly"
     };
     const string msg3 {"big it is. I mean, you may think it's a"
     };
     const string msg4 {"long way down the road to the chemist,"
     };
     const string msg5 {"but that's just peanuts to space."
     };
     const string msg6 {"Douglass Adams - The Hitchhiker's Guide to the Galaxy."
     };

     pair<int,int> size = vterm::get_screen_size();
     vterm::print ( ( size.first/2 )-msg1.size() /2, ( size.second/2 )-3, msg1, color_t {255,255,255}, color_t {0,0,0} );
     vterm::print ( ( size.first/2 )-msg2.size() /2, ( size.second/2 )-2, msg2, color_t {255,255,255}, color_t {0,0,0} );
     vterm::print ( ( size.first/2 )-msg3.size() /2, ( size.second/2 )-1, msg3, color_t {255,255,255}, color_t {0,0,0} );
     vterm::print ( ( size.first/2 )-msg4.size() /2, ( size.second/2 ), msg4, color_t {255,255,255}, color_t {0,0,0} );
     vterm::print ( ( size.first/2 )-msg5.size() /2, ( size.second/2 ) +1, msg5, color_t {255,255,255}, color_t {0,0,0} );
     vterm::print ( ( size.first/2 )-msg6.size() /2, ( size.second/2 ) +3, msg6, color_t {255,255,0}, color_t {0,0,0} );
}

void genesis_mode::begin_genesis ( const double time_elapsed )
{
     intro_quote();
     mode = UGEN;
     total_time = time_elapsed;
}

void genesis_mode::ugen ( const double time_elapsed )
{
     intro_quote();
     unigen.make_universe();

     total_time += time_elapsed;
     if ( total_time > 1000.0 ) {
          mode = UDISPLAY;
          total_time = 0;
     }
}

void genesis_mode::udisplay ( const double time_elapsed )
{
     total_time += time_elapsed;

     if ( total_time < 1500 ) {
          show_universe_map ( false );
     } else {
          show_universe_map ( true );
     }
     const string message = "<- You are here";
     vterm::print ( ( UNIVERSE_WIDTH/2 ) +1, UNIVERSE_HEIGHT/2, message, color_t {0,255,255}, color_t {0,0,0} );

     const string msg1 { "This is just a small part of the universe: Galactic Core."
     };
     const string msg2 { "In the late 21st century, Man spread across the core,"
     };
     const string msg3 { "spreading ruin in its wake."
     };
     const string msg4 { "Worlds burned, and the only cure for war was bigger guns."
     };
     const string msg5 { "Six Houses rose, and decimated the galaxy for their ideology."
     };
     const string msg6 { "Democracy, Trade, Perfection, Magick, Prayer, Nobility."
     };
     const string msg7 { "All good ideas, billions died for their realization."
     };

     pair<int,int> size = vterm::get_screen_size();
     if ( total_time > 500.0 ) vterm::print ( ( size.first/2 )-msg1.size() /2, 0, msg1, color_t {255,255,255}, color_t {0,0,0} );
     if ( total_time > 1000.0 ) {
          vterm::print ( ( size.first/2 )-msg2.size() /2, 1, msg2, color_t {255,255,255}, color_t {0,0,0} );
          if ( !spread_humanity ) {
               unigen.spread_humanity();
               spread_humanity = true;
          }
     }
     if ( total_time > 1500.0 ) vterm::print ( ( size.first/2 )-msg3.size() /2, 2, msg3, color_t {255,255,255}, color_t {0,0,0} );
     if ( total_time > 2000.0 ) {
          vterm::print ( ( size.first/2 )-msg4.size() /2, 3, msg4, color_t {255,255,255}, color_t {0,0,0} );
          unigen.spread_ownership();
     }
     if ( total_time > 2500.0 ) vterm::print ( ( size.first/2 )-msg5.size() /2, 4, msg5, color_t {255,255,255}, color_t {0,0,0} );
     if ( total_time > 3000.0 ) vterm::print ( ( size.first/2 )-msg6.size() /2, 5, msg6, color_t {0,255,255}, color_t {0,0,0} );
     if ( total_time > 3500.0 ) vterm::print ( ( size.first/2 )-msg7.size() /2, 6, msg7, color_t {255,255,255}, color_t {0,0,0} );
     if ( total_time > 5000.0 ) {
          total_time = 0;
          mode = UDISPLAY2;
     }
}

void genesis_mode::udisplay2 ( const double time_elapsed )
{
     total_time += time_elapsed;
     if ( total_time > 1000.0 ) {
          show_universe_map ( true, true );
     } else {
          show_universe_map ( true, false );
     }
     const string message = "<- You are here";
     vterm::print ( ( UNIVERSE_WIDTH/2 ) +1, UNIVERSE_HEIGHT/2, message, color_t {0,255,255}, color_t {0,0,0} );

     const string msg1 { "In 2520, a leader named Blue came to power in Eden."
     };
     const string msg2 { "She brokered a truce, and war-scarred Eden enjoyed some peace."
     };
     const string msg3 { "But Eden was shattered, and the people were slowly dying."
     };
     const string msg4 { "Great arks were built, evacuees chosen by lottery: "
     };
     const string msg5 { "The 'A' ark would carry the best and brightest. "
     };
     const string msg6 { "The 'C' ark would carry harmless criminals. "
     };
     const string msg7 { "The 'B' ark would carry everyone else - "
     };
     const string msg8 { "the wasted; insurance adjusters, hairdressers,"
     };
     const string msg9 { "and that weird guy who is ALWAYS in the pub."
     };

     pair<int,int> size = vterm::get_screen_size();
     if ( total_time > 500.0 ) vterm::print ( ( size.first/2 )-msg1.size() /2, 0, msg1, color_t {0,255,255}, color_t {0,0,0} );
     if ( total_time > 1000.0 ) vterm::print ( ( size.first/2 )-msg2.size() /2, 1, msg2, color_t {0,255,0}, color_t {0,0,0} );
     if ( total_time > 1500.0 ) vterm::print ( ( size.first/2 )-msg3.size() /2, 2, msg3, color_t {255,255,255}, color_t {0,0,0} );
     if ( total_time > 1800.0 ) vterm::print ( ( size.first/2 )-msg4.size() /2, 3, msg4, color_t {255,255,255}, color_t {0,0,0} );
     if ( total_time > 2000.0 ) vterm::print ( ( size.first/2 )-msg5.size() /2, 4, msg5, color_t {255,255,255}, color_t {0,0,0} );
     if ( total_time > 2200.0 ) vterm::print ( ( size.first/2 )-msg6.size() /2, 5, msg6, color_t {255,255,255}, color_t {0,0,0} );
     if ( total_time > 2400.0 ) vterm::print ( ( size.first/2 )-msg7.size() /2, 6, msg7, color_t {255,255,0}, color_t {0,0,0} );
     if ( total_time > 2500.0 ) vterm::print ( ( size.first/2 )-msg8.size() /2, 7, msg8, color_t {255,255,0}, color_t {0,0,0} );
     if ( total_time > 2600.0 ) vterm::print ( ( size.first/2 )-msg9.size() /2, 8, msg9, color_t {255,255,0}, color_t {0,0,0} );

     if ( total_time > 1800.0 and starting_x==0 and starting_y == 0 ) {
          bool ok = false;
          while ( !ok ) {
               starting_x = game_engine->rng.roll_dice ( 1, UNIVERSE_WIDTH/2 );
               starting_y = game_engine->rng.roll_dice ( 1, UNIVERSE_HEIGHT-1 );
               if ( starting_y < 9 ) starting_y = 9;

               const int dx = std::abs ( starting_x - ( UNIVERSE_WIDTH/2 ) );
               const int dy = std::abs ( starting_y - ( UNIVERSE_HEIGHT/2 ) );
               const int distance = std::sqrt ( ( dx*dx ) + ( dy*dy ) );

               if ( distance > 20 ) {
                    const int uidx = universe_idx ( starting_x, starting_y );
                    auto finder = universe->solar_systems.find ( uidx );
                    if ( finder != universe->solar_systems.end() ) {
                         ok = true;
                    }
               }
               universe->globals.starting_system_x = starting_x;
               universe->globals.starting_system_y = starting_y;
          }
     }
     if ( total_time > 1900.0 ) {
          const string message2 = "<- Destination";
          vterm::print ( starting_x+1, starting_y, message2, color_t {0,255,255}, color_t {0,0,0} );
     }
     if ( total_time > 3000.0 ) {
          mode = TRAVEL;
          total_time = 0;
          current_x = UNIVERSE_WIDTH/2;
          current_y = UNIVERSE_HEIGHT/2;
          flight_path.clear();
          flight_progress = 0;
          std::cout << "Destination: " << starting_x << ", " << starting_y << "\n";
          geometry::line_func ( current_x, current_y, starting_x, starting_y, [this] ( const int x, const int y ) {
               this->flight_path.push_back ( std::make_pair ( x, y ) );
          }
                              );
     }
}

void genesis_mode::travel ( const double time_elapsed )
{
     total_time += time_elapsed;
     float flight_progress_f = flight_progress;
     float size_f = flight_path.size();
     float progress = flight_progress_f / size_f;
     if ( progress < 0.5 ) {
          show_universe_map ( true, true );
     } else {
          show_static_map();
     }

     const string msg1 { "Due to 'technical difficulties', the B ark launches first." };
     const string msg2 { "The crew go to sleep, and Cordex (the ship's AI) flies the ship."
     };
     const string msg3 { "At first, it all goes rather well."
     };
     const string msg4 { "We've been hit! Sensors wonky, entering emergency mode!"
     };
     const string msg5 { "We've made it! Scanning the system..."
     };
     pair<int,int> size = vterm::get_screen_size();
     vterm::print ( ( size.first/2 )-msg1.size() /2, 0, msg1, color_t {0,255,255}, color_t {0,0,0} );
     vterm::print ( ( size.first/2 )-msg2.size() /2, 1, msg2, color_t {0,255,255}, color_t {0,0,0} );
     vterm::print ( ( size.first/2 )-msg3.size() /2, 2, msg3, color_t {0,255,255}, color_t {0,0,0} );
     if ( progress > 0.5 ) {
          vterm::print ( ( size.first/2 )-msg4.size() /2, 3, msg4, color_t {255,0,0}, color_t {0,0,0} );
     }
     if ( progress > 0.9 ) {
          vterm::print ( ( size.first/2 )-msg5.size() /2, 4, msg5, color_t {255,255,0}, color_t {0,0,0} );
     }

     if ( total_time > 200 ) {

          ++flight_progress;
          if ( flight_progress < flight_path.size() ) {
               current_x = flight_path [ flight_progress ].first;
               current_y = flight_path [ flight_progress ].second;
               total_time = 0;
          }
     }

     if ( total_time > 500 and progress > 0.9 ) {
          mode = SYSTEM_SCAN;
          const int uidx = universe_idx ( starting_x, starting_y );
          auto finder = universe->solar_systems.find ( uidx );
          solar_system_body_t body;
          body.system_idx = finder->second.bodies.size();
          body.body_type = HABITABLE_PLANET;
          finder->second.bodies.push_back ( body );
     }

     const string message2 = "<- Destination";
     vterm::print ( starting_x+1, starting_y, message2, color_t {0,255,255}, color_t {0,0,0} );

     const string message = "<- You are here";
     vterm::print ( current_x+1, current_y, message, color_t {0,255,255}, color_t {0,0,0} );
}

void genesis_mode::system_scan ( const double time_elapsed )
{
     total_time += time_elapsed;
     const string msg1 { "We've dropped into an unknown system. Scanning..."
     };
     pair<int,int> size = vterm::get_screen_size();
     vterm::print ( ( size.first/2 )-msg1.size() /2, 0, msg1, color_t {0,255,255}, color_t {0,0,0} );

     const int uidx = universe_idx ( starting_x, starting_y );
     auto finder = universe->solar_systems.find ( uidx );
     int i = 1;

     /*
     std::cout << "System scan reveals:\n";
     for ( const solar_system_body_t &body : finder->second.bodies ) {
     std::cout << "Body #" << i << " is";
     switch (body.body_type) {
       case ICE_WORLD : std::cout << " an ice world.\n"; break;
       case VOLCANIC_WORLD : std::cout << " a volcanic world.\n"; break;
       case GAS_CLOUD : std::cout << " a gas cloud.\n"; break;
       case ASTEROIDS : std::cout << " full of asteroids.\n"; break;
       case WARP_HUSK : std::cout << " a warp husk.\n"; break;
       case HABITABLE_PLANET : std::cout << " is an Eden-type planet.\n"; break;
       case SPACE_STATION : std::cout << " a space station.\n"; break;
     }
     ++i;
     }*/

     // Render the sun
     color_t sun_color {255,255,255};

     switch ( finder->second.system_class ) {
     case O :
          sun_color = color_t { 64, 64, 255};
          break;
     case B :
          sun_color = color_t { 128, 128, 255};
          break;
     case A :
          sun_color = color_t  { 192, 192, 255};
          break;
     case F :
          sun_color = color_t { 255, 255, 255};
          break;
     case G :
          sun_color = color_t { 255, 255, 0};
          break;
     case K :
          sun_color = color_t { 128, 128, 0};
          break;
     case M :
          sun_color = color_t { 128, 128, 0};
          break;
     default:
          sun_color = color_t {255,255,255};
          break;
     }

     if ( finder->second.binary_system ) {
          const int mid_y = size.second/2;
          const int sun_1 = mid_y - 6;
          const int sun_2 = mid_y + 6;
          render_sun ( sun_color, sun_1 );
          render_sun ( sun_color, sun_2 );
     } else {
          const int sun_y = size.second/2;
          render_sun ( sun_color, sun_y );
     }

     i = 1;
     for ( const solar_system_body_t &body : finder->second.bodies ) {

          if ( body.body_type == ICE_WORLD ) {
               vterm::screen_character render_char { 219, color_t{0,255,255}, color_t{0,0,0} };
               vterm::set_char_xy ( 9+ ( i*4 ), ( size.second/2 ), render_char );
               vterm::set_char_xy ( 10+ ( i*4 ), ( size.second/2 ), render_char );
               vterm::set_char_xy ( 11+ ( i*4 ), ( size.second/2 ), render_char );
               vterm::set_char_xy ( 10+ ( i*4 ), ( size.second/2 )-1, render_char );
               vterm::set_char_xy ( 10+ ( i*4 ), ( size.second/2 ) +1, render_char );
          } else if ( body.body_type == VOLCANIC_WORLD ) {
               vterm::screen_character render_char { 219, color_t{255, game_engine->rng.roll_dice ( 1,255 ) ,0}, color_t{0,0,0} };
               vterm::set_char_xy ( 9+ ( i*4 ), ( size.second/2 ), render_char );
               vterm::set_char_xy ( 10+ ( i*4 ), ( size.second/2 ), render_char );
               vterm::set_char_xy ( 11+ ( i*4 ), ( size.second/2 ), render_char );
               vterm::set_char_xy ( 10+ ( i*4 ), ( size.second/2 )-1, render_char );
               vterm::set_char_xy ( 10+ ( i*4 ), ( size.second/2 ) +1, render_char );
          } else if ( body.body_type == GAS_CLOUD ) {
               vterm::screen_character render_char { 176, color_t{0,255,0}, color_t{0,0,0} };
               vterm::set_char_xy ( 9+ ( i*4 ), ( size.second/2 ), render_char );
               vterm::set_char_xy ( 10+ ( i*4 ), ( size.second/2 ), render_char );
               vterm::set_char_xy ( 11+ ( i*4 ), ( size.second/2 ), render_char );
               vterm::set_char_xy ( 10+ ( i*4 ), ( size.second/2 )-1, render_char );
               vterm::set_char_xy ( 10+ ( i*4 ), ( size.second/2 ) +1, render_char );
          } else if ( body.body_type == ASTEROIDS ) {
               vterm::screen_character render_char { '*', color_t{255,255,255}, color_t{0,0,0} };
               vterm::set_char_xy ( 10+ ( i*4 ), ( size.second/2 )-1, render_char );
               vterm::set_char_xy ( 10+ ( i*4 ), ( size.second/2 ) +1, render_char );
          } else if ( body.body_type == WARP_HUSK ) {
               vterm::screen_character render_char { 176, color_t{128,0,128}, color_t{0,0,0} };
               vterm::set_char_xy ( 9+ ( i*4 ), ( size.second/2 ), render_char );
               //vterm::set_char_xy( 10+(i*4), (size.second/2), render_char );
               vterm::set_char_xy ( 11+ ( i*4 ), ( size.second/2 ), render_char );
               vterm::set_char_xy ( 10+ ( i*4 ), ( size.second/2 )-1, render_char );
               vterm::set_char_xy ( 10+ ( i*4 ), ( size.second/2 ) +1, render_char );
          } else if ( body.body_type == HABITABLE_PLANET ) {
               vterm::screen_character render_char { 219, color_t{0, 255 ,0}, color_t{0,0,0} };
               vterm::screen_character render_char2 { 219, color_t{0, 0 ,255}, color_t{0,0,0} };
               vterm::set_char_xy ( 9+ ( i*4 ), ( size.second/2 ), render_char );
               vterm::set_char_xy ( 10+ ( i*4 ), ( size.second/2 ), render_char2 );
               vterm::set_char_xy ( 11+ ( i*4 ), ( size.second/2 ), render_char );
               vterm::set_char_xy ( 10+ ( i*4 ), ( size.second/2 )-1, render_char );
               vterm::set_char_xy ( 10+ ( i*4 ), ( size.second/2 ) +1, render_char );
          } else {
               vterm::screen_character render_char { 216, color_t{0,255,255}, color_t{0,0,0} };
               vterm::set_char_xy ( 10+ ( i*4 ), ( size.second/2 ), render_char );
          }
          ++i;
     }
}


void genesis_mode::show_universe_map ( const bool show_warzone, const bool show_truce )
{
     const int size_of_war = 8 + game_engine->rng.roll_dice ( 1, 6 );

     for ( int y=0; y<UNIVERSE_HEIGHT; ++y ) {
          for ( int x=0; x<UNIVERSE_WIDTH; ++x ) {
               const int uidx = universe_idx ( x,y );
               auto finder = universe->solar_systems.find ( uidx );

               const int dx = std::abs ( x - ( UNIVERSE_WIDTH/2 ) );
               const int dy = std::abs ( y - ( UNIVERSE_HEIGHT/2 ) );
               const int distance = std::sqrt ( ( dx*dx ) + ( dy*dy ) );
               color_t background { 0, 0, 0};
               if ( show_warzone and distance < size_of_war ) background = color_t {64 + game_engine->rng.roll_dice ( 1, 64 ), 0, 0};
               if ( show_truce and distance < 5 ) background = color_t {0,128,0};

               if ( finder != universe->solar_systems.end() ) {

                    std::pair<SDL_Color, unsigned char> renderable = solar_system_renderable ( finder->second );
                    vterm::screen_character render_char { renderable.second,
                                                          color_t{ renderable.first.r, renderable.first.g, renderable.first.b },
                                                          background
                                                        };
                    if ( show_warzone and distance < size_of_war ) render_char.character = '@';
                    vterm::set_char_xy ( x, y, render_char );
               } else {
                    vterm::screen_character render_char { ' ',
                                                          background,
                                                          background
                                                        };
                    vterm::set_char_xy ( x, y, render_char );
               }
          }
     }
}

void genesis_mode::show_static_map()
{
     for ( int y=0; y<UNIVERSE_HEIGHT; ++y ) {
          for ( int x=0; x<UNIVERSE_WIDTH; ++x ) {
               color_t background { 0, 0, 0};
               unsigned char s;
               int roll = game_engine->rng.roll_dice ( 1,6 );
               switch ( roll ) {
               case 1 :
                    s = 176;
                    break;
               case 2 :
                    s = 177;
                    break;
               case 3 :
                    s = game_engine->rng.roll_dice ( 1,254 );
                    break;
               default :
                    s = ' ';
                    break;
               }
               const int color = game_engine->rng.roll_dice ( 1,254 );

               vterm::screen_character render_char { s,
                                                     color_t{ color, color , color },
                                                     background
                                                   };
               vterm::set_char_xy ( x, y, render_char );
          }
     }
}

void genesis_mode::render_sun ( engine::vterm::color_t color, const int mid_y )
{
     for ( int i=-3; i<4; ++i ) {
          const int y = mid_y + i;
          for ( int x=0; x<4 - std::abs ( i ); ++x ) {
               vterm::screen_character render_char { 219,
                                                     color,
                                                     color_t{ 0, 0, 0}
                                                   };
               vterm::set_char_xy ( x, y, render_char );
          }
     }
}



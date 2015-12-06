#include "genesis_mode.h"
#include "world/universegen.hpp"

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
    switch (mode) {
      case INIT : begin_genesis(); break;
      case UGEN : ugen(); break;
      case UDISPLAY : udisplay(); break;
    }
  
    
    
    // Temporary
    if ( is_key_down ( ENTER ) ) finished = true;
  
    if (finished) {
	return make_pair ( POP, NO_PUSHED_MODE );
    } else {
	return make_pair ( CONTINUE, NO_PUSHED_MODE );
    }
}

void genesis_mode::begin_genesis()
{
    const string message = "Please wait while the universe initializes";
    pair<int,int> size = vterm::get_screen_size();
    vterm::print( (size.first/2)-message.size()/2, size.second/2, message, color_t{255,255,255}, color_t{0,0,0});
    mode = UGEN;
}

void genesis_mode::ugen()
{
    universe_generator unigen;
    unigen.make_universe();
    mode = UDISPLAY;
}

void genesis_mode::udisplay()
{
    show_universe_map();
}

void genesis_mode::show_universe_map()
{
    for (int y=0; y<UNIVERSE_HEIGHT; ++y) {
	  for (int x=0; x<UNIVERSE_WIDTH; ++x) {
	      const int uidx = universe_idx(x,y);
	      auto finder = universe->solar_systems.find ( uidx );
	      if (finder != universe->solar_systems.end()) {
		  std::pair<SDL_Color, unsigned char> renderable = solar_system_renderable( finder->second );
		  vterm::screen_character render_char { renderable.second, 
		    color_t{ renderable.first.r, renderable.first.g, renderable.first.b },
		    color_t{ 0, 0, 0}
		  };
		  vterm::set_char_xy( x, y, render_char );
	      }
	  }
    }
}


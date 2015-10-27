#include "gui_element_base.h"
#include "../virtual_terminal.h"
#include "../../game/world/world.h"
#include <string>
#include <iostream>
#include <sstream>

using std::string;
using std::stringstream;

namespace engine {
class gui_main_game_command_panel : public gui_element_base {
public:
     gui_main_game_command_panel ( const string name ) {
          tag = name;
     };;

     virtual void render ( const screen_region viewport ) {
	  int x = viewport.x+1;
	  int y = viewport.y+1;
	  
	  vterm::print(x,y,world::display_day_month,white,black);
	  vterm::print(x,y+1,world::display_time,white,black);
	  vterm::print(x,y+2,world::display_season,white,black);
	  
	  stringstream ss;
	  ss << "Power: " << world::stored_power << " / " << world::max_power;
	  vterm::print(x,y+4,ss.str(),white,black);
	  ss.clear();
	  
	  int max_width = viewport.w-1;
	  const float power_percent = float(world::stored_power) / float(world::max_power);
	  float ticks = max_width * power_percent;
	  const vterm::screen_character display_good{219, green, black};
	  const vterm::screen_character display_bad{250, dark_green, black};
	  for (int i=0; i<max_width; i++) {
	      if (i <= ticks) {
		  vterm::set_char_xy(x+i, y+5, display_good);
	      } else {
		  vterm::set_char_xy(x+i, y+5, display_bad);
	      }
	  }
	  
     }

private:
};
}

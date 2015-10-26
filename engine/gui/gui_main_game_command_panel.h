#include "gui_element_base.h"
#include "../virtual_terminal.h"
#include "../../game/world/world.h"
#include <string>
#include <iostream>

using std::string;

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
     }

private:
};
}

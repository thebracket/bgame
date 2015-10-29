#include <string>
#include <iostream>
#include <sstream>
#include "../../engine/gui/gui_element_base.h"
#include "../../game/world/world.h"

using std::string;
using std::stringstream;

namespace engine {
  
class gui_main_game_command_panel : public gui_element_base {
public:
     gui_main_game_command_panel ( const string name );
     virtual void render ( const screen_region viewport );

};
}

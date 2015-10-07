#include "gui_element_base.h"
#include "../virtual_terminal.h"
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
     }

private:
};
}

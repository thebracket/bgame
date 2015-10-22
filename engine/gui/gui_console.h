#include "gui_element_base.h"
#include "../virtual_terminal.h"
#include <string>
#include <iostream>
#include "../../game/world/world.h"

using std::string;

namespace engine {
class gui_console : public gui_element_base {
public:  
     gui_console ( const string name ) {
          tag = name;
     };;

     virtual void render ( const screen_region viewport );

private:
};
}

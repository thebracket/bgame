#include "gui_element_base.h"
#include "../virtual_terminal.h"
#include "../../game/world/world.h"
#include <string>
#include <iostream>
#include <algorithm>
#include "../../engine/ecs/ecs.h"
#include "../ecs/components/position_component.h"
#include <cmath>

using std::string;

namespace engine {
class gui_main_game_view : public gui_element_base {
public:
     gui_main_game_view ( const string name ) {
          tag = name;
     };

     virtual void render ( const screen_region viewport );

private:
};
}

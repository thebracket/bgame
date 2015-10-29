#include <iostream>
#include <memory>
#include "engine/engine.h"
#include "test_mode.h"
#include "game/loading_screen.h"
#include "config.h"
#include "engine/globals.h"

#include "game/components/component_types.h"
#include "game/components/calendar_component.h"
#include "game/components/debug_name_component.h"
#include "game/components/obstruction_component.h"
#include "game/components/position_component.h"
#include "game/components/power_battery_component.h"
#include "game/components/power_generator_component.h"
#include "game/components/renderable_component.h"
#include "game/components/settler_ai_component.h"
#include "game/components/viewshed_component.h"

using std::cout;
using std::make_unique;

template<typename T>
void component_loader(fstream &lbfile) {
  T component;
  component.load(lbfile);
  const int entity_handle = component.entity_id;
  entity * parent = engine::globals::ecs->get_entity_by_handle ( entity_handle );
  if (parent == nullptr) {
    std::cout << "Error! Entity " << component.entity_id << " is null.\n";
  } else {
    engine::globals::ecs->add_component<T>(*parent, component);
  }
}

void component_factory(fstream &lbfile, const int ct) {
  switch ( ct ) {
    case engine::ecs::component_type::position : component_loader<engine::ecs::position_component>(lbfile); break;
      case engine::ecs::component_type::name : component_loader<engine::ecs::debug_name_component>(lbfile); break;
      case engine::ecs::component_type::renderable : component_loader<engine::ecs::renderable_component>(lbfile); break;
      case engine::ecs::component_type::viewshed :  component_loader<engine::ecs::viewshed_component>(lbfile); break;
      case engine::ecs::component_type::calendar : component_loader<engine::ecs::calendar_component>(lbfile); break;
      case engine::ecs::component_type::settler_ai : component_loader<engine::ecs::settler_ai_component>(lbfile); break;
      case engine::ecs::component_type::obstruction : component_loader<engine::ecs::obstruction_component>(lbfile); break;
      case engine::ecs::component_type::power_generator : component_loader<engine::ecs::power_generator_component>(lbfile); break;
      case engine::ecs::component_type::power_battery : component_loader<engine::ecs::power_battery_component>(lbfile); break;
      default :
	    throw 102;
  }
}

int main()
{
     GAME engine;
     GAME_ECS ecs(component_factory);
     engine::globals::ecs = &ecs;
     engine.init();
     engine.main_loop( make_unique<loading_screen>() );
}


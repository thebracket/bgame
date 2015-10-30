#pragma once

#include "component_types.h"
#include "calendar_component.h"
#include "debug_name_component.h"
#include "obstruction_component.h"
#include "position_component.h"
#include "power_battery_component.h"
#include "power_generator_component.h"
#include "renderable_component.h"
#include "settler_ai_component.h"
#include "viewshed_component.h"
#include "game_stats_component.h"
#include "game_species_component.h"
#include "game_health_component.h"

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

inline void component_factory(fstream &lbfile, const int ct) {
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
      case engine::ecs::component_type::gamestats : component_loader<engine::ecs::game_stats_component>(lbfile); break;
      case engine::ecs::component_type::gamespecies : component_loader<engine::ecs::game_species_component>(lbfile); break;
      case engine::ecs::component_type::gamehealth : component_loader<engine::ecs::game_health_component>(lbfile); break;
      default :
	    throw 102;
  }
}

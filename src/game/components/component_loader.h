#pragma once

#include "components.h"
#include "../game.h"

using engine::entity;

template<typename T>
void component_loader(fstream &lbfile)
{
	T component;
	component.load(lbfile);
	const int entity_handle = component.entity_id;
	entity * parent = ECS->get_entity_by_handle(entity_handle);
	if (parent == nullptr)
	{
		std::cout << "Error! Entity " << component.entity_id << " is null.\n";
	}
	else
	{
		ECS->add_component<T>(*parent, component);
	}
}

inline void component_factory(fstream &lbfile, const int ct)
{
	switch (ct)
	{
	case name:
		component_loader<debug_name_component>(lbfile);
		break;
	case renderable:
		component_loader<renderable_component>(lbfile);
		break;
	case viewshed:
		component_loader<viewshed_component>(lbfile);
		break;
	case calendar:
		component_loader<calendar_component>(lbfile);
		break;
	case settler_ai:
		component_loader<settler_ai_component>(lbfile);
		break;
	case obstruction:
		component_loader<obstruction_component>(lbfile);
		break;
	case power_generator:
		component_loader<power_generator_component>(lbfile);
		break;
	case power_battery:
		component_loader<power_battery_component>(lbfile);
		break;
	case gamestats:
		component_loader<game_stats_component>(lbfile);
		break;
	case gamespecies:
		component_loader<game_species_component>(lbfile);
		break;
	case gamehealth:
		component_loader<game_health_component>(lbfile);
		break;
	case provision:
		component_loader<provisions_component>(lbfile);
		break;
	case item_storage:
		component_loader<item_storage_component>(lbfile);
		break;
	case item_carried:
		component_loader<item_carried_component>(lbfile);
		break;
	case description:
		component_loader<description_component>(lbfile);
		break;
	case item:
		component_loader<item_component>(lbfile);
		break;
	case tree:
		component_loader<tree_component>(lbfile);
		break;
	case particle_emitter:
		component_loader<particle_emitter_component>(lbfile);
		break;
	case position3d:
		component_loader<position_component3d>(lbfile);
		break;
	case walkable_roof:
		component_loader<walkable_roof_component>(lbfile);
		break;
	case pointlight:
		component_loader<point_light_component>(lbfile);
		break;
	case parent_entity:
		component_loader<parent_entity_component>(lbfile);
		break;
	case stairs:
		component_loader<stairs_component>(lbfile);
	default:
		throw 102;
	}
}

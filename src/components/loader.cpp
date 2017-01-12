#include "loader.hpp"
#include "world_position.hpp"
#include "position.hpp"
#include "calendar.hpp"
#include "renderable.hpp"
#include "name.hpp"
#include "species.hpp"
#include "health.hpp"
#include "game_stats.hpp"
#include "item.hpp"
#include "item_carried.hpp"
#include "settler_ai.hpp"
#include "item_stored.hpp"
#include "designations.hpp"
#include "construct_container.hpp"
#include "construct_power.hpp"
#include "construct_provides_sleep.hpp"
#include "viewshed.hpp"
#include "building.hpp"
#include "lightsource.hpp"
#include "grazer_ai.hpp"
#include "wildlife_group.hpp"
#include "corpse_settler.hpp"
#include "corpse_harvestable.hpp"
#include "smoke_emitter.hpp"
#include "sentient_ai.hpp"
#include "logger.hpp"
#include "water_spawner.hpp"
#include "falling.hpp"
#include "construct_provides_door.hpp"
#include "renderable_composite.hpp"
#include "initiative.hpp"
#include "slidemove.hpp"
#include "stockpile.hpp"
#include "bridge.hpp"

#include <rltk.hpp>
#include <iostream>

template<typename T>
void inner_loader(rltk::xml_node * node, std::size_t entity_id) {
	T component;
	component.from_xml(node);
	entity(entity_id)->assign(std::move(component));
}

void component_loader_xml(rltk::xml_node * node, std::size_t entity_id, std::string xml_identity) {
	if (xml_identity == "world_position_t") {
		inner_loader<world_position_t>(node, entity_id);
	} else if (xml_identity == "position_t") {
		inner_loader<position_t>(node, entity_id);
	} else if (xml_identity == "calendar_t") {
		inner_loader<calendar_t>(node, entity_id);
	} else if (xml_identity == "renderable_t") {
		inner_loader<renderable_t>(node, entity_id);
	} else if (xml_identity == "name_t") {
		inner_loader<name_t>(node, entity_id);
	} else if (xml_identity == "species_t") {
		inner_loader<species_t>(node, entity_id);
	} else if (xml_identity == "game_stats_t") {
		inner_loader<game_stats_t>(node, entity_id);
	} else if (xml_identity == "item_t") {
		inner_loader<item_t>(node, entity_id);
	} else if (xml_identity == "item_carried_t") {
		inner_loader<item_carried_t>(node, entity_id);
	} else if (xml_identity == "settler_ai_t") {
		inner_loader<settler_ai_t>(node, entity_id);
	} else if (xml_identity == "item_stored_t") {
		inner_loader<item_stored_t>(node, entity_id);
	} else if (xml_identity == "designations_t") {
		inner_loader<designations_t>(node, entity_id);
	} else if (xml_identity == "construct_container_t") {
		inner_loader<construct_container_t>(node, entity_id);
	} else if (xml_identity == "construct_power_t") {
		inner_loader<construct_power_t>(node, entity_id);
	} else if (xml_identity == "construct_provides_sleep_t") {
		inner_loader<construct_provides_sleep_t>(node, entity_id);
	} else if (xml_identity == "viewshed_t") {
		inner_loader<viewshed_t>(node, entity_id);
	} else if (xml_identity == "building_t") {
		inner_loader<building_t>(node, entity_id);
	} else if (xml_identity == "lightsource_t") {
		inner_loader<lightsource_t>(node, entity_id);
	} else if (xml_identity == "grazer_ai") {
		inner_loader<grazer_ai>(node, entity_id);
	} else if (xml_identity == "wildlife_group") {
		inner_loader<wildlife_group>(node, entity_id);
	} else if (xml_identity == "corpse_settler") {
		inner_loader<corpse_settler>(node, entity_id);
	} else if (xml_identity == "corpse_harvestable") {
		inner_loader<corpse_harvestable>(node, entity_id);
	} else if (xml_identity == "smoke_emitter_t") {
		inner_loader<smoke_emitter_t>(node, entity_id);
	} else if (xml_identity == "sentient_ai") {
		inner_loader<sentient_ai>(node, entity_id);
	} else if (xml_identity == "logger_t") {
		inner_loader<logger_t>(node, entity_id);
	} else if (xml_identity == "water_spawner_t") {
		inner_loader<water_spawner_t>(node, entity_id);
	} else if (xml_identity == "falling_t") {
		inner_loader<falling_t>(node, entity_id);
	} else if (xml_identity == "construct_door_t") {
		inner_loader<construct_door_t>(node, entity_id);
	} else if (xml_identity == "health_t") {
		inner_loader<health_t>(node, entity_id);
	} else if (xml_identity == "renderable_composite_t") {
		inner_loader<renderable_composite_t>(node, entity_id);
	} else if (xml_identity == "initiative_t") {
		inner_loader<initiative_t>(node, entity_id);
	} else if (xml_identity == "slidemove_t") {
        inner_loader<slidemove_t>(node, entity_id);
	} else if (xml_identity == "stockpile_t") {
		inner_loader<stockpile_t>(node, entity_id);
	} else if (xml_identity == "bridge_t") {
		inner_loader<bridge_t>(node, entity_id);
    } else {
		throw std::runtime_error("Unknown component type, "+xml_identity);
	}
}

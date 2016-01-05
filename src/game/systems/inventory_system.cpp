#include "inventory_system.h"
#include "../game.h"
#include "../world/universe.hpp"

void inventory_system::store_inventory(debug_name_component * name,
		position_component3d * pos)
{
	auto finder = universe->globals.inventory.find(name->debug_name);
	if (finder == universe->globals.inventory.end())
	{
		available_item_t item{ name->debug_name, pos->pos, name->entity_id };
		universe->globals.inventory[name->debug_name] = vector<available_item_t>{ item };
	}
	else
	{
		available_item_t item{ name->debug_name, pos->pos, name->entity_id };
		finder->second.push_back(item);
	}
}

void inventory_system::register_inventory_stored(item_storage_component * store)
{
	debug_name_component * name = ECS->find_entity_component<debug_name_component>(store->entity_id);
	position_component3d * pos = ECS->find_entity_component<position_component3d>(store->container_id);
	store_inventory(name, pos);
}

void inventory_system::register_inventory_carried(
		item_carried_component * carried)
{
	debug_name_component * name = ECS->find_entity_component<debug_name_component>(carried->entity_id);
	position_component3d * pos = ECS->find_entity_component<position_component3d>(carried->carried_by_id);
	store_inventory(name, pos);
}

void inventory_system::register_inventory_ground(position_component3d * pos)
{
	debug_name_component * name = ECS->find_entity_component<debug_name_component>(pos->entity_id);
	store_inventory(name, pos);
}

// FIXME: This needs to limit indexing to the current region
void inventory_system::tick(const double& duration_ms)
{
	bool need_inventory_refresh = false;
	if (universe->globals.inventory.empty()) {
		need_inventory_refresh = true;
	}
	vector<item_changed_message> * item_changes = game_engine->messaging->get_messages_by_type<item_changed_message>();
	for (item_changed_message &msg : *item_changes)
	{
		msg.deleted = true;
		need_inventory_refresh = true;
	}

	if (!need_inventory_refresh)
		return;

	universe->globals.inventory.clear();
	vector<item_component> * items = ECS->find_components_by_type<item_component>();
	for (const item_component &item : *items)
	{
		if (!item.claimed)
		{
			item_storage_component * store = ECS->find_entity_component<item_storage_component>(item.entity_id);
			item_carried_component * carried = ECS->find_entity_component<item_carried_component>(item.entity_id);
			position_component3d * on_ground = ECS->find_entity_component<position_component3d>(item.entity_id);

			if (store != nullptr)
			{
				register_inventory_stored(store);
			}
			else if (carried != nullptr)
			{
				register_inventory_carried(carried);
			}
			else if (on_ground != nullptr)
			{
				register_inventory_ground(on_ground);
			}
		}
	}
}


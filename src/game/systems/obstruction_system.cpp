#include "obstruction_system.h"
#include "../components/obstruction_component.h"
#include "../components/walkable_roof_component.h"
#include "../game.h"
#include "../world/planet.hpp"
#include "../world/universe.hpp"

void obstruction_system::tick(const double& duration_ms)
{
	bool changes = false;
	vector<walkability_changed_message> * change_list =
			game_engine->messaging->get_messages_by_type<
					walkability_changed_message>();
	for (walkability_changed_message &msg : *change_list)
	{
		changes = true;
		msg.deleted = true;
	}
	if (changes)
	{
		region_t * current_region = get_current_region();
		current_region->calculate_walkability();

		// Obstructions from components
		const vector<obstruction_component> * blockages =
				ECS->find_components_by_type<obstruction_component>();
		for (const obstruction_component &block : *blockages)
		{
			position_component3d * pos = ECS->find_entity_component<
					position_component3d>(block.entity_id);
			if (pos != nullptr)
			{
				//const int idx = get_tile_index( pos->pos.x, pos->pos.y, pos->pos.z );
				tile_t * tile = world::planet->get_tile(pos->pos);
				if (block.blocks_entry)
					tile->flags.set(TILE_OPTIONS::WALK_BLOCKED);
				if (block.blocks_visibility)
					tile->flags.set(TILE_OPTIONS::VIEW_BLOCKED);
			}
		}

		// Walkable roofs
		const vector<walkable_roof_component> * roofs =
				ECS->find_components_by_type<walkable_roof_component>();
		for (const walkable_roof_component &roof : *roofs)
		{
			position_component3d * pos = ECS->find_entity_component<
					position_component3d>(roof.entity_id);
			if (pos != nullptr)
			{
				location_t up = pos->pos;
				up.z++;
				current_region->get_tile(up.x, up.y, up.z)->flags.set(
						TILE_OPTIONS::CAN_STAND_HERE);
			}
		}
	}
}

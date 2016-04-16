#include "damage_system.hpp"

#include "../game.h"
#include "../world/skill_test.h"
#include "../world/universe.hpp"

void damage_system::tick(const double& duration_ms)
{
	vector<inflict_damage_message> * damages =
			game_engine->messaging->get_messages_by_type<inflict_damage_message>();
	for (inflict_damage_message &damage : *damages)
	{
		if (!damage.deleted)
		{
			game_health_component * health = ECS->find_entity_component<
					game_health_component>(damage.target_entity);
			if (health != nullptr)
			{
				health->current_hit_points -= damage.damage_taken;
				std::cout << "Target takes " << damage.damage_taken
						<< " hit points of damage. Cause: "
						<< damage.damage_source << "\n";
				if (health->current_hit_points < 0)
					std::cout
							<< "If death were implemented, that would have been fatal!\n";
				// TODO: Blood splatters
				std::stringstream msg;
				msg << (0 - damage.damage_taken);
				game_engine->messaging->add_message<particle_message>(
						particle_message(msg.str(), damage.tile_x,
								damage.tile_y, damage.tile_z, 64, RED));
			}
			else
			{
				std::cout
						<< "Warning: Attempting to inflict damage on something with no hitpoints!\n";
			}
		}
		damage.deleted = true;
	}

	if (!universe->globals.paused)
	{
		calendar_component * calendar = ECS->find_entity_component<
				calendar_component>(universe->globals.cordex_handle);

		if (calendar->minute % 30 == 0)
		{

			vector<game_health_component> * health =
					ECS->find_components_by_type<game_health_component>();
			for (game_health_component &h : *health)
			{
				if (h.current_hit_points < h.max_hit_points)
				{
					if (game_system::skill_roll(h.entity_id, "Self Heal", 12)
							>= game_system::SUCCESS)
					{
						++h.current_hit_points;
						position_component3d * pos = ECS->find_entity_component<
								position_component3d>(h.entity_id);
						game_engine->messaging->add_message<particle_message>(
								particle_message("+1", pos->pos.x, pos->pos.y,
										pos->pos.z, 64, GREEN));
					}
				}
			}

		}
	}
}

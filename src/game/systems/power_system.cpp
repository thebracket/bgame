#include "power_system.h"
#include "../components/power_battery_component.h"
#include "../components/power_generator_component.h"
#include "../components/calendar_component.h"
#include "../world/world.h"
#include <iostream>
#include "../game.h"
#include "../messages/power_consumed_message.h"
#include "../world/universe.hpp"

namespace power_system_detail
{

int calculate_power_gain(const power_generator_component* gen)
{
	if (gen->generator_mode == raws::DAYLIGHT)
	{
		// TODO: Once the sun is restored, calculate solar efficiency
		const float generated = gen->amount;
		return generated;
	}
	return 0;
}

}

void power_system::tick(const double& duration_ms)
{
	if (universe->globals.paused)
		return;

	calendar_component * calendar = ECS->find_entity_component<calendar_component>(universe->globals.cordex_handle);

	if (last_tick + 10 < calendar->system_time or last_tick == 0)
	{
		int power = universe->globals.stored_power;

		const vector<power_generator_component> * producers =
				ECS->find_components_by_type<power_generator_component>();
		for (const power_generator_component &gen : *producers)
		{
			const int generated_power =
					power_system_detail::calculate_power_gain(&gen);
			power += generated_power;
		}

		const vector<power_battery_component> * storage =
				ECS->find_components_by_type<power_battery_component>();
		int storage_capacity = 0;
		for (const power_battery_component &bat : *storage)
		{
			storage_capacity += bat.storage_capacity;
		}

		// Power consumption from messages
		vector<power_consumed_message> * consumption_ptr =
				game_engine->messaging->get_messages_by_type<
						power_consumed_message>();
		for (power_consumed_message &usage : *consumption_ptr)
		{
			power -= usage.quantity;
			usage.deleted = true;
			//std::cout << "Power consumption occurred: " << usage.quantity << "\n";
		}

		if (power > storage_capacity)
			power = storage_capacity;
		if (power < 0)
			power = 0; // TODO: Dead!
		universe->globals.stored_power = power;
		universe->globals.max_power = storage_capacity;
		// TODO: If power < 0 - dead!
		last_tick = calendar->system_time;
	}
}


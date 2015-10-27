#include "power_system.h"
#include "../ecs.h"
#include "../power_battery_component.h"
#include "../power_generator_component.h"
#include "../calendar_component.h"
#include "../../../game/world/world.h"
#include <iostream>

void engine::ecs::power_system::tick(const double& duration_ms)
{
    entity * cordex = get_entity_by_handle ( world::cordex_handle );
    int calendar_handle = cordex->find_component_by_type ( calendar );
    calendar_component * calendar = get_component_by_handle<calendar_component> ( calendar_handle );

    if (last_tick+5 < calendar->system_time or last_tick == 0) {
        int power = world::stored_power;

        const vector<power_generator_component *> producers = find_components_by_type<power_generator_component> ( power_generator );
        for (const power_generator_component * gen : producers) {
            const int generated_power = calculate_power_gain(gen);
            power += generated_power;
        }

        const vector<power_battery_component *> storage = find_components_by_type<power_battery_component> ( power_battery );
        int storage_capacity = 0;
        for (const power_battery_component * bat : storage) {
            storage_capacity += bat->storage_capacity;
        }

        // TODO: Power consumption!

        if (power > storage_capacity) power = storage_capacity;
        world::stored_power = power;
	world::max_power = storage_capacity;
        // TODO: If power < 0 - dead!
	last_tick = calendar->system_time;
    }
}

int engine::ecs::power_system::calculate_power_gain(const engine::ecs::power_generator_component* gen)
{
    if (gen->generator_mode == raws::DAYLIGHT) {
        float efficiency = 0.0F;
        if (world::sun_angle <= 90.0F) {
            efficiency = 1 - (90.0F - world::sun_angle);
        } else {
            efficiency = 1 - (world::sun_angle-90.0F);
        }
        if (efficiency < 0.1) efficiency = 0.1F;
        float generated = gen->amount * efficiency;
        return generated;
    }
    return 0;
}


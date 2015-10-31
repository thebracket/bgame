#include "power_system.h"
#include "../components/power_battery_component.h"
#include "../components/power_generator_component.h"
#include "../components/calendar_component.h"
#include "../world/world.h"
#include <iostream>
#include "../../engine/globals.h"

namespace power_system_detail {

int calculate_power_gain(const power_generator_component* gen)
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

}

void power_system::tick(const double& duration_ms)
{
    if (world::paused) return;
    
    calendar_component * calendar = engine::globals::ecs->find_entity_component<calendar_component>(world::cordex_handle);

    if (last_tick+5 < calendar->system_time or last_tick == 0) {
        int power = world::stored_power;

        const vector<power_generator_component> * producers = engine::globals::ecs->find_components_by_type<power_generator_component> ();
        for (const power_generator_component &gen : *producers) {
            const int generated_power = power_system_detail::calculate_power_gain(&gen);
            power += generated_power;
        }

        const vector<power_battery_component> * storage = engine::globals::ecs->find_components_by_type<power_battery_component> ();
        int storage_capacity = 0;
        for (const power_battery_component &bat : *storage) {
            storage_capacity += bat.storage_capacity;
        }

        // TODO: Power consumption!

        if (power > storage_capacity) power = storage_capacity;
        world::stored_power = power;
	world::max_power = storage_capacity;
        // TODO: If power < 0 - dead!
	last_tick = calendar->system_time;
    }
}


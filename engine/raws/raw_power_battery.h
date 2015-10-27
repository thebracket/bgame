#pragma once

#include "base_raw.h"
#include "../ecs/power_battery_component.h"

namespace engine {
namespace raws {

struct raw_power_battery : public base_raw {
    raw_power_battery() {
        type = POWER_BATTERY;
    };
    raw_power_battery(const int &capacity) : storage_capacity(capacity) {
        type = POWER_BATTERY;
    };
    int storage_capacity;

    void build_components(engine::ecs::entity &parent, const int &x, const int &y) const {
        unique_ptr<ecs::power_battery_component> battery = make_unique<ecs::power_battery_component>(storage_capacity);
        engine::ecs::add_component(parent, std::move(battery));
    }
};

}
}

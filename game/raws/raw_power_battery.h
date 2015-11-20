#pragma once

#include "base_raw.h"
#include "../../game/components/power_battery_component.h"
#include "../game.h"

namespace raws {

struct raw_power_battery : public base_raw {
    raw_power_battery() {
        type = POWER_BATTERY;
    };
    raw_power_battery(const int &capacity) : storage_capacity(capacity) {
        type = POWER_BATTERY;
    };
    int storage_capacity;

    void build_components(entity &parent, const int &x, const int &y, const bool placeholder=false) const {
        if (!placeholder) game_engine->ecs->add_component(parent, power_battery_component(storage_capacity));
    }
};

}

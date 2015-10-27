#pragma once

#include "base_component.h"

namespace engine {
namespace ecs {

class power_battery_component : public base_component {
public:
    power_battery_component();
    power_battery_component(const int &new_amount);

    int storage_capacity;

    virtual void save(fstream &lbfile);
    virtual void load(fstream &lbfile);
};

}
}
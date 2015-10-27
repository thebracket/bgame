#pragma once

#include "base_component.h"

namespace engine {
namespace ecs {

class power_battery_component : public base_component {
public:
    power_battery_component() {
        type = power_battery;
    };
    power_battery_component(const int &new_amount) : storage_capacity(new_amount) {
        type = power_battery;
    };

    int storage_capacity;

    virtual void save(fstream &lbfile) {
        base_component::save(lbfile);
        lbfile.write ( reinterpret_cast<const char *> ( &storage_capacity ), sizeof ( storage_capacity ) );
    };

    virtual void load(fstream &lbfile) {
        base_component::load(lbfile);
        lbfile.read ( reinterpret_cast<char *> ( &storage_capacity ), sizeof ( storage_capacity ) );
    };
};

}
}
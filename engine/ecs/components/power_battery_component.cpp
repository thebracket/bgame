#include "power_battery_component.h"

namespace engine {
namespace ecs {

power_battery_component::power_battery_component() {
    type = power_battery;
}

power_battery_component::power_battery_component(const int &new_amount) : storage_capacity(new_amount) {
    type = power_battery;
}

void power_battery_component::save(fstream &lbfile) {
    base_component::save(lbfile);
    lbfile.write ( reinterpret_cast<const char *> ( &storage_capacity ), sizeof ( storage_capacity ) );
}

void power_battery_component::load(fstream &lbfile) {
    base_component::load(lbfile);
    lbfile.read ( reinterpret_cast<char *> ( &storage_capacity ), sizeof ( storage_capacity ) );
}

}
}
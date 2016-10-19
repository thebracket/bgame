#include "water_spawner.hpp"
#include "../utils/serialization_wrapper.hpp"

void water_spawner_t::save(std::ostream &lbfile) {
    Serialize(lbfile, spawner_type);
}

water_spawner_t water_spawner_t::load(std::istream &lbfile) {
    water_spawner_t c;
    Deserialize(lbfile, c.spawner_type);
    return c;
}

#include "construct_power.hpp"
#include "../utils/serialization_wrapper.hpp"

void construct_power_t::save(std::ostream &lbfile) {
    Serialize(lbfile, storage_capacity, generation_always, generation_solar);
}

construct_power_t construct_power_t::load(std::istream &lbfile) {
    construct_power_t c;
    Deserialize(lbfile, c.storage_capacity, c.generation_always, c.generation_solar);
    return c;
}
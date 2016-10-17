#include "item_carried.hpp"
#include "../utils/serialization_wrapper.hpp"

void item_carried_t::save(std::ostream &lbfile) {
    Serialize("item_carried_t", lbfile, location, carried_by);
}

item_carried_t item_carried_t::load(std::istream &lbfile) {
    item_carried_t c;
    Deserialize("item_carried_t", lbfile, c.location, c.carried_by);
    return c;
}
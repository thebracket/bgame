#include "item_stored.hpp"
#include "../utils/serialization_wrapper.hpp"

void item_stored_t::save(std::ostream &lbfile) {
    Serialize(lbfile, stored_in);
}

item_stored_t item_stored_t::load(std::istream &lbfile) {
    item_stored_t c;
    Deserialize(lbfile, c.stored_in);
    return c;
}
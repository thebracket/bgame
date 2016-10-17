#include "name.hpp"
#include "../utils/serialization_wrapper.hpp"

void name_t::save(std::ostream &lbfile) {
    Serialize(lbfile, first_name, last_name);
}

name_t name_t::load(std::istream &lbfile) {
    name_t c;
    Deserialize(lbfile, c.first_name, c.last_name);
    return c;
}

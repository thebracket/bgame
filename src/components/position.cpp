#include "position.hpp"
#include "../utils/serialization_wrapper.hpp"

void position_t::save(std::ostream &lbfile) {
    Serialize(lbfile, x, y, z);
}

void position_t::load(std::istream &lbfile) {
    Deserialize(lbfile, x, y, z);
}
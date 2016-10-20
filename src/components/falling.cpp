#include "falling.hpp"
#include "../utils/serialization_wrapper.hpp"

void falling_t::save(std::ostream &lbfile) {
    Serialize(lbfile, distance);
};

falling_t falling_t::load(std::istream &lbfile) {
    falling_t c;
    Deserialize(lbfile, c.distance);
    return c;
};
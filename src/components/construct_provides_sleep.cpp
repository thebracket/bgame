#include "construct_provides_sleep.hpp"
#include "../utils/serialization_wrapper.hpp"

void construct_provides_sleep_t::save(std::ostream &lbfile) {
    Serialize(lbfile, claimed);
}

construct_provides_sleep_t construct_provides_sleep_t::load(std::istream &lbfile) {
    construct_provides_sleep_t c;
    Deserialize(lbfile, c.claimed);
    return c;
}

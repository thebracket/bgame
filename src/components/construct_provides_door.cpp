#include "construct_provides_door.hpp"
#include "../utils/serialization_wrapper.hpp"

void construct_door_t::save(std::ostream &lbfile) {
    Serialize("construct_provides_door", lbfile, locked);
}

construct_door_t construct_door_t::load(std::istream &lbfile) {
    construct_door_t c;
    Deserialize("construct_provides_door", lbfile, c.locked);
    return c;
}

#include "construct_provides_door.hpp"
#include "../utils/serialization_wrapper.hpp"

void construct_door_t::save(std::ostream &lbfile) {
    Serialize("construct_provides_door", lbfile, civ_owner, locked);
}

construct_door_t construct_door_t::load(std::istream &lbfile) {
    construct_door_t c;
    Deserialize("construct_provides_door", lbfile, c.civ_owner, c.locked);
    return c;
}

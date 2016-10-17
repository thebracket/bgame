#include "corpse_settler.hpp"
#include "../utils/serialization_wrapper.hpp"

void corpse_settler::save(std::ostream &lbfile) {
    Serialize(lbfile, ticks_since_death, cause_of_death);
}

corpse_settler corpse_settler::load(std::istream &lbfile) {
    corpse_settler c;
    Deserialize(lbfile, c.ticks_since_death, c.cause_of_death);
    return c;
}

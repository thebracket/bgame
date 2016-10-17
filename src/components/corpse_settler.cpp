#include "corpse_settler.hpp"
#include "../utils/serialization_wrapper.hpp"

void corpse_settler::save(std::ostream &lbfile) {
    Serialize("corpse_settler", lbfile, ticks_since_death, cause_of_death);
}

corpse_settler corpse_settler::load(std::istream &lbfile) {
    corpse_settler c;
    Deserialize("corpse_settler", lbfile, c.ticks_since_death, c.cause_of_death);
    return c;
}

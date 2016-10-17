#include "corpse_harvestable.hpp"
#include "../utils/serialization_wrapper.hpp"

void corpse_harvestable::save(std::ostream &lbfile) {
    Serialize("corpse_harvestable", lbfile, ticks_since_death, creature_tag);
}

corpse_harvestable corpse_harvestable::load(std::istream &lbfile) {
    corpse_harvestable c;
    Deserialize("corpse_harvestable", lbfile, c.ticks_since_death, c.creature_tag);
    return c;
}
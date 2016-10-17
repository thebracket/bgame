#include "corpse_harvestable.hpp"
#include "../utils/serialization_wrapper.hpp"

void corpse_harvestable::save(std::ostream &lbfile) {
    Serialize(lbfile, ticks_since_death, creature_tag);
}

corpse_harvestable corpse_harvestable::load(std::istream &lbfile) {
    corpse_harvestable c;
    Deserialize(lbfile, c.ticks_since_death, c.creature_tag);
    return c;
}
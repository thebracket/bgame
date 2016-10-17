#include "sentient_ai.hpp"
#include "../utils/serialization_wrapper.hpp"

void sentient_ai::save(std::ostream &lbfile) {
    Serialize("sentient_ai",lbfile, initiative, initiative_modifier, person_id, aggression, hostile, goal, target, days_since_arrival);
}

sentient_ai sentient_ai::load(std::istream &lbfile) {
    sentient_ai c;
    Deserialize("sentient_ai", lbfile, c.initiative, c.initiative_modifier, c.person_id, c.aggression, c.hostile, c.goal, c.target, c.days_since_arrival);
    return c;
}

#include "health.hpp"
#include "../utils/serialization_wrapper.hpp"

void health_t::save(std::ostream &lbfile) {
    Serialize("health_t", lbfile, max_hitpoints, current_hitpoints, unconscious, blind, immobile, slow, no_grasp, parts);
}

health_t health_t::load(std::istream &lbfile) {
    health_t c;
    Deserialize("health_t", lbfile, c.max_hitpoints, c.current_hitpoints, c.unconscious, c.blind, c.immobile, c.slow, c.no_grasp, c.parts);
    return c;
}
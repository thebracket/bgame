#include "health_part_t.hpp"
#include "../../utils/serialization_wrapper.hpp"

void health_part_t::save(std::ostream &f) {
    Serialize("health_part_t", f, part, max_hitpoints, current_hitpoints, size);
}

void health_part_t::load(std::istream &f) {
    Deserialize("health_part_t", f, part, max_hitpoints, current_hitpoints, size);
}

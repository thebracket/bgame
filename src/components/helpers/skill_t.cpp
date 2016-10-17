#include "skill_t.hpp"
#include "../../utils/serialization_wrapper.hpp"

void skill_t::save(std::ostream &f) {
    Serialize("skill_t", f, skill_level, experience_gained);
}

void skill_t::load(std::istream &f) {
    Deserialize("skill_t", f, skill_level, experience_gained);
}
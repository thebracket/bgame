#include "reaction_task_t.hpp"
#include "../../utils/serialization_wrapper.hpp"

void reaction_task_t::save(std::ostream &f) {
    Serialize(f, building_id, job_name, reaction_tag, components);
}

void reaction_task_t::load(std::istream &f) {
    Deserialize(f, building_id, job_name, reaction_tag, components);
}

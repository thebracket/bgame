#include "settler_ai.hpp"
#include "../utils/serialization_wrapper.hpp"

void settler_ai_t::save(std::ostream &lbfile) {
    Serialize("settler_ai_t", lbfile, initiative, shift_id, job_type_major, job_status, job_type_minor, target_x, target_y, target_z, target_id, building_target, reaction_target, current_tool);
    for (const bool &b : permitted_work) {
        Serialize("settier_ai_t_work", lbfile, b);
    }
}

settler_ai_t settler_ai_t::load(std::istream &lbfile) {
    settler_ai_t c;
    Deserialize("settler_ai_t", lbfile, c.initiative, c.shift_id, c.job_type_major, c.job_status, c.job_type_minor, c.target_x, c.target_y, c.target_z, c.target_id, c.building_target, c.reaction_target, c.current_tool);
    for (bool &b : c.permitted_work) {
        Deserialize("settler_ai_t_work", lbfile, b);
    }
    return c;
}
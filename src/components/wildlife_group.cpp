#include "wildlife_group.hpp"
#include "../utils/serialization_wrapper.hpp"

void wildlife_group::save(std::ostream &lbfile) {
    Serialize("wildlife_group", lbfile, group_id);
}

wildlife_group wildlife_group::load(std::istream &lbfile) {
    wildlife_group c;
    Deserialize("wildlife_group", lbfile, c.group_id);
    return c;
}

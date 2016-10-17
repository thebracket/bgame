#include "building.hpp"
#include "../utils/serialization_wrapper.hpp"

void building_t::save(std::ostream &lbfile) {
    Serialize(lbfile, tag, width, height, complete, glyphs, built_with);
}

building_t building_t::load(std::istream &lbfile) {
    building_t c;
    Deserialize(lbfile, c.tag, c.width, c.height, c.complete, c.glyphs, c.built_with);
    return c;
}

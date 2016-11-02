#include "building.hpp"
#include "../utils/serialization_wrapper.hpp"

void building_t::save(std::ostream &lbfile) {
    Serialize("building_t", lbfile, tag, width, height, complete, glyphs, built_with, civ_owner);
}

building_t building_t::load(std::istream &lbfile) {
    building_t c;
    Deserialize("building_t", lbfile, c.tag, c.width, c.height, c.complete, c.glyphs, c.built_with, c.civ_owner);
    return c;
}

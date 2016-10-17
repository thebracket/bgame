#include "building_designation_t.hpp"
#include "../../utils/serialization_wrapper.hpp"

void building_designation_t::save(std::ostream &f) {
    Serialize("building_designation_t", f, x, y, z, component_ids, name, tag, components, width, height, glyphs, building_entity);
}

void building_designation_t::load(std::istream &f) {
    Deserialize("building_designation_t", f, x, y, z, component_ids, name, tag, components, width, height, glyphs, building_entity);
}

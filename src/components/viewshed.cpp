#include "viewshed.hpp"
#include "../utils/serialization_wrapper.hpp"

void viewshed_t::save(std::ostream &lbfile) {
    Serialize(lbfile, viewshed_radius, penetrating, good_guy_visibility, visible_entities);
}

viewshed_t viewshed_t::load(std::istream &lbfile) {
    viewshed_t c;
    Deserialize(lbfile, c.viewshed_radius, c.penetrating, c.good_guy_visibility, c.visible_entities);
    return c;
}

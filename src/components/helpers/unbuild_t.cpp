#include "unbuild_t.hpp"

void unbuild_t::to_xml(rltk::xml_node * c) {
    c->add_value("is_building", std::to_string(is_building));
    c->add_value("building_id", std::to_string(building_id));
}

void unbuild_t::from_xml(rltk::xml_node * c) {
    is_building = c->val<bool>("is_building");
    building_id = c->val<std::size_t>("building_id");
}
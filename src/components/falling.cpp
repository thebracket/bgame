#include "falling.hpp"

void falling_t::to_xml(xml_node * c) {
    component_to_xml(c, std::make_pair("distance", distance));
}

void falling_t::from_xml(xml_node * c) {
    distance = c->val<int>("distance");
}

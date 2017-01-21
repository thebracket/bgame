#include "lever.hpp"

void lever_t::to_xml(xml_node * c) {
    component_to_xml(c, std::make_pair("active", active));
}

void lever_t::from_xml(xml_node * c) {
    active = c->val<bool>("active");
}

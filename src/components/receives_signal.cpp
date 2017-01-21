#include "receives_signal.hpp"

void receives_signal_t::to_xml(xml_node * c) {
    component_to_xml(c, std::make_pair("active", active));
}

void receives_signal_t::from_xml(xml_node * c) {
    active = c->val<bool>("active");
}

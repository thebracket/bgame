#include "entry_trigger.hpp"

void entry_trigger_t::to_xml(xml_node * c) {
    component_to_xml(c, std::make_pair("active", active), std::make_pair("type", type));
}

void entry_trigger_t::from_xml(xml_node * c) {
    active = c->val<bool>("active");
    type = (trigger_type )c->val<int>("type");
}

#include "construct_provides_sleep.hpp"

void construct_provides_sleep_t::to_xml(xml_node * c) {
    component_to_xml(c, std::make_pair("claimed", claimed));
}

void construct_provides_sleep_t::from_xml(xml_node * c) {
    claimed = c->val<bool>("claimed");
}
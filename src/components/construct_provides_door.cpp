#include "construct_provides_door.hpp"

void construct_door_t::to_xml(xml_node * c) {
    component_to_xml(c, std::make_pair("locked", locked));
}

void construct_door_t::from_xml(xml_node * c) {
    locked = c->val<bool>("locked");
}

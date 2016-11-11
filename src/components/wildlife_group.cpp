#include "wildlife_group.hpp"

void wildlife_group::to_xml(xml_node * c) {
    component_to_xml(c, std::make_pair("group_id", group_id));
}

void wildlife_group::from_xml(xml_node * c) {
    group_id = c->val<uint8_t>("group_id");
}

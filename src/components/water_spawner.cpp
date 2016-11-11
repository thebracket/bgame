#include "water_spawner.hpp"

void water_spawner_t::to_xml(xml_node * c) {
    component_to_xml(c,
        std::make_pair("spawner_type", spawner_type)
    );
}

void water_spawner_t::from_xml(xml_node * c) {
    spawner_type = c->val<uint8_t>("spawner_type");
}
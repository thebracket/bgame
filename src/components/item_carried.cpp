#include "item_carried.hpp"

void item_carried_t::to_xml(xml_node * c) {
    component_to_xml(c,
        std::make_pair("location", location),
        std::make_pair("carried_by", carried_by)
    );
}

void item_carried_t::from_xml(xml_node * c) {
    location = (item_location_t)c->val<int>("location");
    carried_by = c->val<std::size_t>("carried_by");
}
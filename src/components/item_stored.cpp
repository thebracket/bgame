#include "item_stored.hpp"

void item_stored_t::to_xml(xml_node * c) {
    component_to_xml(c,
        std::make_pair("stored_in", stored_in)
    );
}

void item_stored_t::from_xml(xml_node * c) {
    stored_in = c->val<std::size_t>("stored_in");
}
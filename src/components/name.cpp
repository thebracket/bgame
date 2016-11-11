#include "name.hpp"

void name_t::to_xml(xml_node * c) {
    component_to_xml(c,
        std::make_pair("first_name", first_name),
        std::make_pair("last_name", last_name)
    );
}

void name_t::from_xml(xml_node * c) {
    std::stringstream ss;

    first_name = c->val<std::string>("first_name");
    last_name = c->val<std::string>("last_name");
}
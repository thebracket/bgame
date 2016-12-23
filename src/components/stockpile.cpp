#include "stockpile.hpp"

void stockpile_t::to_xml(xml_node *c) {
    component_to_xml(c,
                     std::make_pair("category", category)
    );
}

void stockpile_t::from_xml(xml_node *c) {
    std::string cats = c->val<std::string>("category");
    category = std::bitset<128>(cats);
}
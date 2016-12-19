#include "position.hpp"

void position_t::to_xml(xml_node * c) {
    component_to_xml(c,
        std::make_pair("x", x), 
        std::make_pair("y", y), 
        std::make_pair("z", z)
    );
}

void position_t::from_xml(xml_node * c) {
    x = c->val<float>("x");
    y = c->val<float>("y");
    z = c->val<int>("z");
}

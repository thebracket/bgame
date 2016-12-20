#include "position.hpp"

void position_t::to_xml(xml_node * c) {
    component_to_xml(c,
        std::make_pair("x", x), 
        std::make_pair("y", y), 
        std::make_pair("z", z),
        std::make_pair("offsetX", offsetX),
        std::make_pair("offsetY", offsetY),
        std::make_pair("offsetZ", offsetZ)
    );
}

void position_t::from_xml(xml_node * c) {
    x = c->val<int>("x");
    y = c->val<int>("y");
    z = c->val<int>("z");
    offsetX = c->val<float>("offsetX");
    offsetY = c->val<float>("offsetX");
    offsetZ = c->val<float>("offsetX");
}

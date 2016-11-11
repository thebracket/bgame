#include "world_position.hpp"

void world_position_t::to_xml(xml_node * c) {
    component_to_xml(c,
        std::make_pair("world_x", world_x), 
        std::make_pair("world_y", world_y), 
        std::make_pair("region_x", region_x), 
        std::make_pair("region_y", region_y), 
        std::make_pair("region_z", region_z)
    );
}

void world_position_t::from_xml(xml_node * c) {
    world_x = c->val<int>("world_x");
    world_y = c->val<int>("world_y");
    region_x = c->val<int>("region_x");
    region_y = c->val<int>("region_y");
    region_z = c->val<int>("region_z");
}

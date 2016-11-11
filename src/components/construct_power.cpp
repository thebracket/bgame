#include "construct_power.hpp"

void construct_power_t::to_xml(xml_node * c) {
    component_to_xml(c,
        std::make_pair("storage_capacity", storage_capacity), 
        std::make_pair("generation_always", generation_always), 
        std::make_pair("generation_solar", generation_solar)
    );
}

void construct_power_t::from_xml(xml_node * c) {
    storage_capacity = c->val<int>("storage_capacity");
    generation_always = c->val<int>("generation_always");
    generation_solar = c->val<int>("generation_solar");
}
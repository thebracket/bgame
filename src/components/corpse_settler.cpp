#include "corpse_settler.hpp"

void corpse_settler::to_xml(xml_node * c) {
    component_to_xml(c, 
        std::make_pair("ticks_since_death", ticks_since_death),
        std::make_pair("cause_of_death", cause_of_death)
    );
}

void corpse_settler::from_xml(xml_node * c) {
    ticks_since_death = c->val<uint32_t>("ticks_since_death");
    cause_of_death = c->val<std::string>("cause_of_death");
}

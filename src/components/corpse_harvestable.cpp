#include "corpse_harvestable.hpp"

void corpse_harvestable::to_xml(xml_node * c) {
    component_to_xml(c, 
        std::make_pair("ticks_since_death", ticks_since_death),
        std::make_pair("creature_tag", creature_tag)
    );
}

void corpse_harvestable::from_xml(xml_node * c) {
    ticks_since_death = c->val<uint32_t>("ticks_since_death");
    creature_tag = c->val<std::string>("creature_tag");
}

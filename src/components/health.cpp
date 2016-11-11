#include "health.hpp"

void health_t::to_xml(xml_node * c) {
    component_to_xml(c,
        std::make_pair("max_hitpoints", max_hitpoints),
        std::make_pair("current_hitpoints", current_hitpoints),
        std::make_pair("unconscious", unconscious),
        std::make_pair("blind", blind),
        std::make_pair("immobile", immobile),
        std::make_pair("slow", slow),
        std::make_pair("no_grasp", no_grasp),
        std::make_pair("parts", parts)
    );
}

void health_t::from_xml(xml_node * c) {
    max_hitpoints = c->val<int>("max_hitpoints");
    current_hitpoints = c->val<int>("current_hitpoints");
    unconscious = c->val<bool>("unconscious");
    blind = c->val<bool>("blind");
    immobile = c->val<bool>("immobile");
    slow = c->val<bool>("slow");
    no_grasp = c->val<bool>("no_grasp");
    c->iterate_child("parts", [this] (xml_node * c) {
        health_part_t p;
        p.from_xml(c);
        parts.push_back(p);
    });
}
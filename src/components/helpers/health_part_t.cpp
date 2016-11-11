#include "health_part_t.hpp"

void health_part_t::to_xml(rltk::xml_node * c) {
    c->add_value("part", part);
    c->add_value("max_hitpoints", std::to_string(max_hitpoints));
    c->add_value("current_hitpoints", std::to_string(current_hitpoints));
    c->add_value("size", std::to_string(size));
}

void health_part_t::from_xml(rltk::xml_node * c) {
    part = c->val<std::string>("part");
    max_hitpoints = c->val<int>("max_hitpoints");
    current_hitpoints = c->val<int>("current_hitpoints");
    size = c->val<int>("size");
}

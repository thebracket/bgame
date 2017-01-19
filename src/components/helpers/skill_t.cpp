#include "skill_t.hpp"

void skill_t::to_xml(rltk::xml_node * c) {
    c->add_value("skill_level", std::to_string(skill_level));
    std::cout << "Saved skill level " << skill_level << "\n";
    c->add_value("experience_gained", std::to_string(experience_gained));
}

void skill_t::from_xml(rltk::xml_node * c) {
    skill_level = c->val<int16_t>("skill_level");
    std::cout << "Loaded skill level " << skill_level << "\n";
    experience_gained = c->val<uint16_t>("experience_gained");
}
#include "shift.hpp"

using namespace rltk;

void shift_t::to_xml(xml_node * c) {
    xml_node * s = c->add_node("shift");
    s->add_value("name", shift_name);
    for (int i=0; i<24; ++i) {
        s->add_value(std::to_string(i), std::to_string(hours[i]));
    }
}

void shift_t::from_xml(xml_node * c) {
    shift_name = c->val<std::string>("name");
    for (int i=0; i<24; ++i) {
        int h = c->val<int>(std::to_string(i));
        hours[i] = (shift_type_t)h;
    }
}
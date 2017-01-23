#include "lever.hpp"

void lever_t::to_xml(xml_node * c) {
    component_to_xml(c, std::make_pair("active", active),
                        std::make_pair("targets", targets));
}

void lever_t::from_xml(xml_node * c) {
    active = c->val<bool>("active");
    targets.clear();
    c->iterate_child("targets", [this] (xml_node * s) {
        targets.push_back(s->val<std::size_t>("targets"));
    });
}

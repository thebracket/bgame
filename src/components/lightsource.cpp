#include "lightsource.hpp"

void lightsource_t::to_xml(xml_node * c) {
    component_to_xml(c,
        std::make_pair("radius", radius),
        std::make_pair("color", color),
        std::make_pair("alert_status", alert_status)
    );
}

void lightsource_t::from_xml(xml_node * c) {
    radius = c->val<int>("radius");
    color = c->color("color");
    alert_status = c->val<bool>("alert_status");
}
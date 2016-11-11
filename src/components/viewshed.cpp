#include "viewshed.hpp"

void viewshed_t::to_xml(xml_node * c) {
    component_to_xml(c,
        std::make_pair("viewshed_radius", viewshed_radius),
        std::make_pair("penetrating", penetrating),
        std::make_pair("good_guy_visibility", good_guy_visibility),
        std::make_pair("visible_entities", visible_entities)
    );
}

void viewshed_t::from_xml(xml_node * c) {
    viewshed_radius = c->val<int>("viewshed_radius");
    penetrating = c->val<bool>("penetrating");
    good_guy_visibility = c->val<bool>("good_guy_visibility");
    c->iterate_child("visible_entities", [this] (xml_node * ch) {
        // TODO
    });
}

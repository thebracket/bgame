#include "renderable.hpp"

void renderable_t::to_xml(xml_node * c) {
    component_to_xml(c,
        std::make_pair("glyph", glyph),
        std::make_pair("foreground", foreground),
        std::make_pair("background", background)
    );
}

void renderable_t::from_xml(xml_node * c) {
    glyph = c->val<uint8_t>("glyph");
    foreground = c->color("foreground");
    background = c->color("background");
}
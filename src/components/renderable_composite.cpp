#include "renderable_composite.hpp"

void renderable_composite_t::to_xml(xml_node * c) {
    rltk::component_to_xml(c, std::make_pair("render_mode", render_mode));
}

void renderable_composite_t::from_xml(rltk::xml_node * c) {
    render_mode = (render_mode_t)c->val<int>("render_mode");
}
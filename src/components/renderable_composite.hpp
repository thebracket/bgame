#pragma once

#include <rltk.hpp>
#include <vector>

using namespace rltk;

enum render_mode_t { RENDER_SETTLER };

struct renderable_composite_t {
    render_mode_t render_mode = RENDER_SETTLER;
    std::string xml_identity = "renderable_composite_t";

    renderable_composite_t() {}
    renderable_composite_t(const render_mode_t &rm) : render_mode(rm) {}

    void to_xml(xml_node * c);
    void from_xml(xml_node * c);
};
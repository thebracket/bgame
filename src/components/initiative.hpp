#pragma once

#include <rltk.hpp>

struct initiative_t {
    initiative_t() {}
    initiative_t(const int init_mod) : initiative_modifier(init_mod) {}
    int initiative = 0;
    int initiative_modifier = 0;

    std::string xml_identity = "initiative_t";

    void to_xml(rltk::xml_node * c);
    void from_xml(rltk::xml_node * c);
};
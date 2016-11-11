#pragma once

#include <rltk.hpp>
#include <vector>

using namespace rltk;

struct building_t {
    building_t(const std::string ntag, const int w, const int h, const std::vector<rltk::vchar> &g, const bool comp, const std::size_t owner=0) :
        tag(ntag), width(w), height(h), glyphs(g), complete(comp), civ_owner(owner) {}

    std::string tag;
    int width, height;
    std::vector<rltk::vchar> glyphs;
    bool complete = false;
    std::vector<std::pair<std::string, std::size_t>> built_with;
    std::size_t civ_owner=0;

	building_t() {}

    std::string xml_identity = "building_t";

    void to_xml(xml_node * c);
    void from_xml(xml_node * c);
};
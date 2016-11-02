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

	std::size_t serialization_identity = 18;

    void save(std::ostream &lbfile);
    static building_t load(std::istream &lbfile);
};
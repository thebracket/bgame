#pragma once

#include <rltk.hpp>
#include <vector>

using namespace rltk;

struct building_t {
    building_t(const std::string ntag, const int w, const int h, const std::vector<rltk::vchar> &g, const bool comp) :
        tag(ntag), width(w), height(h), glyphs(g), complete(comp) {}

    std::string tag;
    int width, height;
    std::vector<rltk::vchar> glyphs;
    bool complete = false;

	building_t() {}

	std::size_t serialization_identity = 18;

	void save(std::ostream &lbfile) {
        serialize(lbfile, tag);
        serialize(lbfile, width);
        serialize(lbfile, height);
        std::size_t sz = glyphs.size();
        serialize(lbfile, sz);
        for (const rltk::vchar &glyph : glyphs) {
            serialize(lbfile, glyph);
        }
	}

	static building_t load(std::istream &lbfile) {
		building_t c;
        deserialize(lbfile, c.tag);
        deserialize(lbfile, c.width);
        deserialize(lbfile, c.height);
        std::size_t sz;
        deserialize(lbfile, sz);
        for (std::size_t i=0; i<sz; ++i) {
            rltk::vchar glyph;
            deserialize(lbfile, glyph);
            c.glyphs.push_back(glyph);
        }
		return c;
	}
};
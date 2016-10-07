#include "building.hpp"

void building_t::save(std::ostream &lbfile) {
    serialize(lbfile, tag);
    serialize(lbfile, width);
    serialize(lbfile, height);
    serialize(lbfile, complete);
    std::size_t sz = glyphs.size();
    serialize(lbfile, sz);
    for (const rltk::vchar &glyph : glyphs) {
        serialize(lbfile, glyph);
    }
    sz = built_with.size();
    serialize(lbfile, sz);
    for (const auto &c : built_with) {
        serialize(lbfile, c.first);
        serialize(lbfile, c.second);
    }
}

building_t building_t::load(std::istream &lbfile) {
    building_t c;
    deserialize(lbfile, c.tag);
    deserialize(lbfile, c.width);
    deserialize(lbfile, c.height);
    deserialize(lbfile, c.complete);
    std::size_t sz;
    deserialize(lbfile, sz);
    for (std::size_t i=0; i<sz; ++i) {
        rltk::vchar glyph;
        deserialize(lbfile, glyph);
        c.glyphs.push_back(glyph);
    }
    deserialize(lbfile, sz);
    for (std::size_t i=0; i<sz; ++i) {
        std::string component;
        std::size_t material;
        deserialize(lbfile, component);
        deserialize(lbfile, material);
        c.built_with.push_back(std::make_pair(component, material));
    }
    return c;
}
#include "building.hpp"

void building_t::save(std::ostream &lbfile) {
    serialize(lbfile, tag);
    serialize(lbfile, width);
    serialize(lbfile, height);
    std::size_t sz = glyphs.size();
    serialize(lbfile, sz);
    for (const rltk::vchar &glyph : glyphs) {
        serialize(lbfile, glyph);
    }
}

building_t building_t::load(std::istream &lbfile) {
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
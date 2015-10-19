#pragma once

#include "base_component.h"
#include "../virtual_terminal.h"
#include <vector>

using std::vector;

namespace engine {
namespace ecs {

class renderable_component : public base_component {
public:
    renderable_component() {
        type = renderable;
	multi_tile = false;
    };
    
    renderable_component(const unsigned char &GLYPH, const engine::vterm::color_t &FOREGROUND, const engine::vterm::color_t &BACKGROUND) :
        glyph(GLYPH), foreground(FOREGROUND), background(BACKGROUND)
    {
        type = renderable;
	multi_tile = false;
    };
    
    renderable_component(const engine::vterm::color_t &FOREGROUND, const engine::vterm::color_t &BACKGROUND,
	  const char &nwidth, const char &nheight, const vector<unsigned char> &nglyphs
    ) :
        foreground(FOREGROUND), background(BACKGROUND), width(nwidth), height(nheight)
    {
        type = renderable;
	multi_tile = true;
	for (const unsigned char &c : nglyphs) glyphs.push_back(c);
    };
    
    unsigned char glyph;
    engine::vterm::color_t foreground;
    engine::vterm::color_t background;
    
    // Multi-tile support
    bool multi_tile;
    char width;
    char height;
    vector<unsigned char> glyphs;
};

}
}

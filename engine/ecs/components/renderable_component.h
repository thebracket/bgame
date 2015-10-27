#pragma once

#include "base_component.h"
#include "../../virtual_terminal.h"
#include <vector>
#include <iostream>

using std::vector;

namespace engine {
namespace ecs {

class renderable_component : public base_component {
public:
    renderable_component();    
    renderable_component(const unsigned char GLYPH, const engine::vterm::color_t &FOREGROUND, const engine::vterm::color_t &BACKGROUND);
    
    renderable_component(const engine::vterm::color_t &FOREGROUND, const engine::vterm::color_t &BACKGROUND,
	  const char &nwidth, const char &nheight, const vector<unsigned char> &nglyphs
    );
    
    unsigned char glyph;
    engine::vterm::color_t foreground;
    engine::vterm::color_t background;
    
    // Multi-tile support
    bool multi_tile;
    char width;
    char height;
    vector<unsigned char> glyphs;
    
     virtual void save(fstream &lbfile);    
    virtual void load(fstream &lbfile);
};

}
}

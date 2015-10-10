#pragma once

#include "../../engine/virtual_terminal.h"
#include <fstream>
#include "serialization_utils.h"

using engine::vterm::color_t;
using std::fstream;

struct tile_material {
public:
     color_t background {0,0,0};
     color_t foreground {255,0,0};
     unsigned char glyph = '.';
     void save (fstream &lbfile ) {
	save_color(background, lbfile);
	save_color(foreground, lbfile);
	char tmp = static_cast<char>(glyph);
	lbfile.write(&tmp, 1);
     }

     // TODO: Insert some actual material properties
         
};

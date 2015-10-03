#include "gui_element_base.h"
#include "../virtual_terminal.h"
#include <string>
#include <iostream>

using std::string;

namespace engine {
class gui_static_text : public gui_element_base {
public:
     gui_static_text ( const string &txt, const int &nx, const int &ny, const vterm::color_t &fg, const vterm::color_t &bg ) :
	text(txt), x(nx), y(ny), foreground(fg), background(bg) {};
	
     virtual void render() {
       vterm::print(x,y,text,foreground,background);
     }
private:
     const string text;
     const int x;
     const int y;
     const vterm::color_t foreground;
     const vterm::color_t background;
};
}

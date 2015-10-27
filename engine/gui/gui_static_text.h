#include "gui_element_base.h"
#include "../virtual_terminal.h"
#include <string>
#include <iostream>

using std::string;

namespace engine {
class gui_static_text : public gui_element_base {
public:
    gui_static_text ( const string &name,
		      const string &txt,
                      const int &nx, const int &ny, const vterm::color_t &fg,
                      const vterm::color_t &bg,
                      bool center=false
                    ) :
        centered(center),text(txt), x(nx), y(ny), foreground(fg), background(bg)
        
    {
      tag = name;
    };

    virtual void render(const screen_region viewport);
    void update_text(const string s);
    void move(const int &nx, const int &ny);
    void change_color(const vterm::color_t fg, const vterm::color_t bg);
    
private:
    const bool centered;
    string text;
    int x;
    int y;
    vterm::color_t foreground;
    vterm::color_t background;
};
}

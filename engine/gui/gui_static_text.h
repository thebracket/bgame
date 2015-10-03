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
        text(txt), x(nx), y(ny), foreground(fg), background(bg),
        centered(center)
    {
      tag = name;
    };

    virtual void render(const screen_region viewport) {
        if (!centered) {
            vterm::print(x,y,text,foreground,background);
        } else {
            int text_width = text.size();
            vterm::print(viewport.x + (viewport.w/2)-(text_width/2),y,text,foreground,background);
        }
    }

    void update_text(const string s) {
        text = s;
    }
    void move(const int &nx, const int &ny) {
        x=nx;
        y=ny;
    }
    void change_color(const vterm::color_t fg, const vterm::color_t bg) {
        foreground = fg;
        background = bg;
    }
private:
    const bool centered;
    string text;
    int x;
    int y;
    vterm::color_t foreground;
    vterm::color_t background;
};
}

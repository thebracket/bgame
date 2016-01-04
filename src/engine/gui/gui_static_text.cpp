#include "gui_static_text.h"

namespace engine {

void gui_static_text::render(const screen_region viewport) {
    if (!centered) {
        vterm::print(x,y,text,foreground,background);
    } else {
        int text_width = text.size();
        vterm::print(viewport.x + (viewport.w/2)-(text_width/2),y,text,foreground,background);
    }
}

void gui_static_text::update_text(const string s) {
    text = s;
}
void gui_static_text::move(const int &nx, const int &ny) {
    x=nx;
    y=ny;
}
void gui_static_text::change_color(const vterm::color_t fg, const vterm::color_t bg) {
    foreground = fg;
    background = bg;
}

}
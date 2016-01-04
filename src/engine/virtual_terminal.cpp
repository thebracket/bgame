#include "virtual_terminal.h"
#include <algorithm>
#include <memory>
#include <iostream>

using std::unique_ptr;
using std::make_unique;
using std::fill;

namespace engine {

namespace vterm {

const screen_character blank_character
{' '
};

int screen_width;
int screen_height;
int n_chars;
unique_ptr<vector<screen_character>> terminal_buffer;

inline int screen_idx ( const int x, const int y )
{
     return ( y*screen_width ) +x;
}

inline void set_char ( const int idx, const screen_character &c )
{
     terminal_buffer->operator[] ( idx ) = c;
}

void set_char_xy ( const int x, const int y, const engine::vterm::screen_character& c ) {
  set_char(screen_idx(x,y),c);
}

pair<int,int> get_screen_size() {
  return std::make_pair(screen_width, screen_height);
}

void clear_screen()
{
     fill ( terminal_buffer->begin(), terminal_buffer->end(), blank_character );
}

void print ( const int x, const int y, const string text, const color_t fg, color_t bg )
{
     int idx = screen_idx ( x,y );
     for ( unsigned int i=0; i<text.size(); ++i ) {
          set_char ( idx+i, {static_cast<const unsigned char>(text[i]), fg, bg} );
     }
}

void draw_box ( const int &x, const int &y, const int &w, const int &h, const color_t fg, const color_t bg )
{
    set_char_xy(x,y,{218,fg,bg});
    set_char_xy(x+w,y,{191,fg,bg});
    set_char_xy(x,y+h,{192,fg,bg});
    set_char_xy(x+w,y+h,{217,fg,bg});
    for (int i=y+1; i<(y+h); ++i) {
        set_char_xy(x,i,{179,fg,bg});
        set_char_xy(x+w,i,{179,fg,bg});
    }
    for (int i=x+1; i<(x+w); ++i) {
        set_char_xy(i,y,{196,fg,bg});
        set_char_xy(i,y+h,{196,fg,bg});
    }
}

void draw_dbl_box ( const int &x, const int &y, const int &w, const int &h, const color_t fg, const color_t bg )
{
    set_char_xy(x,y,{201,fg,bg});
    set_char_xy(x+w,y,{187,fg,bg});
    set_char_xy(x,y+h,{200,fg,bg});
    set_char_xy(x+w,y+h,{188,fg,bg});
    for (int i=y+1; i<(y+h); ++i) {
        set_char_xy(x,i,{186,fg,bg});
        set_char_xy(x+w,i,{186,fg,bg});
    }
    for (int i=x+1; i<(x+w); ++i) {
        set_char_xy(i,y,{205,fg,bg});
        set_char_xy(i,y+h,{205,fg,bg});
    }
}

void resize ( const int new_width, const int new_height )
{
     screen_width = new_width;
     screen_height = new_height;
     n_chars = screen_width * screen_height;
     terminal_buffer->resize ( n_chars );
}

vector<screen_character> * get_virtual_screen()
{
     return terminal_buffer.get();
}
} // end vterm namespace

void init_virtual_terminal()
{
     // Temporary
     engine::vterm::terminal_buffer = make_unique<vector<engine::vterm::screen_character>>();
     engine::vterm::resize ( 80,25 );
     engine::vterm::clear_screen();
}


}

#include "ansi_backend.h"
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <ncurses.h>
#include "curses_charmap.h"

using std::make_pair;
using std::stringstream;

namespace engine {

namespace ansi {
  
inline void hide_cursor(stringstream &out) {
    out << "\033[?25l";
}

inline void show_cursor(stringstream &out) {
    out << "\033[?25h";
}
  
inline void curpos(std::stringstream &out, const int &x, const int &y) {
    out << "\033[" << y+1 << ";" << x+1 << "H";
}

inline void foreground(stringstream &out, const std::tuple<unsigned char, unsigned char, unsigned char> &col) {
    const int red = std::get<0>(col);
    const int green = std::get<1>(col);
    const int blue = std::get<2>(col);
    out << "\033[38;2;" << red << ";" << green << ";" << blue << "m";
}

inline void background(stringstream &out, const std::tuple<unsigned char, unsigned char, unsigned char> &col) {
    const int red = std::get<0>(col);
    const int green = std::get<1>(col);
    const int blue = std::get<2>(col);
    out << "\033[48;2;" << red << ";" << green << ";" << blue << "m";
}

} // End ANSI namespace
  
ansi_backend::ansi_backend()
{

}

ansi_backend::~ansi_backend()
{

}

void ansi_backend::init()
{
    setlocale(LC_ALL, "");
    
    std::pair<int,int> dimensions = get_console_size();
    screen_width = dimensions.first;
    screen_height = dimensions.second;
    
    initscr();
    raw();
    nonl();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    cbreak();
    nodelay(stdscr, TRUE);
}

void ansi_backend::stop()
{
    stringstream out;
    ansi::foreground(out, engine::vterm::color_t{255,255,255});
    ansi::background(out, engine::vterm::color_t{0,0,0});
    std::cout << out.str();
    endwin();
}

pair< int, int > ansi_backend::get_console_size()
{
    // Old C interface - sorry!
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    return make_pair(ws.ws_col, ws.ws_row);
}

void ansi_backend::draw ( vector< vterm::screen_character >* screen )
{
    stringstream output;
    ansi::hide_cursor(output);
  
    std::tuple<unsigned char,unsigned char,unsigned char> current_fg{255,255,255};
    std::tuple<unsigned char,unsigned char,unsigned char> current_bg{0,0,0};
    
    for (int y=0; y<screen_height; ++y) {
	ansi::curpos(output, 0, y);
        for (int x=0; x<screen_width; ++x) {
	    auto fg = screen->operator[]((y*screen_width)+x).foreground_color;
            auto bg = screen->operator[]((y*screen_width)+x).background_color;
	    
	    if (fg != current_fg) {
		ansi::foreground(output, fg);
		current_fg = fg;
	    }
	    if (bg != current_bg) {
		ansi::background(output, bg);
		current_bg = bg;
	    }
	    
	    int base_ascii = screen->operator[]((y*screen_width)+x).character;
	    output << curses::charmap[base_ascii];
        }
    }
    std::cout << output.str();
}

command::keys ansi_backend::translate_keycode(const int &key) {
    //std::cout << key << "\n";
    switch (key) {
    case KEY_UP :
        return command::UP;
    case KEY_DOWN :
        return command::DOWN;
    case KEY_LEFT :
        return command::LEFT;
    case KEY_RIGHT :
        return command::RIGHT;
    case KEY_ENTER :
        return command::ENTER;
    case 13 :
        return command::ENTER;
    case 'Q' :
        return command::Q;
    case 'q' :
        return command::Q;
    }
    return command::NONE;
}

void ansi_backend::poll_inputs()
{
    int ch = getch();
    if (ch != ERR) {
        // We have input to read
        command::on_command( {translate_keycode(ch), 0, 0, command::KEYDOWN});
    }
}

  
  
}

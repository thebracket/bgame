#include "ncurses_backend.h"
#include <ncurses.h>

namespace engine {

ncurses_backend::ncurses_backend()
{
    initialized = false;
}

ncurses_backend::~ncurses_backend()
{
  stop();
}

void ncurses_backend::init()
{
    initialized = true;
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    getmaxyx(stdscr, screen_height, screen_width);
}

void ncurses_backend::stop()
{
    if (initialized) {
        endwin();
    }
    initialized = false;
}


pair< int, int > ncurses_backend::get_console_size()
{
    int rows, cols;
    getmaxyx(stdscr,rows,cols);
    return std::make_pair(cols,rows);
}

void ncurses_backend::draw(vector< vterm::screen_character >* screen)
{
    for (int y=0; y<screen_height; ++y) {
        for (int x=0; x<screen_width; ++x) {
            mvaddch(y, x, screen->operator[]((y*screen_width)+x).character);
        }
    }
    refresh();
}


}

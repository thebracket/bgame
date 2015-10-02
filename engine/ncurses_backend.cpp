#include "ncurses_backend.h"
#include <ncurses.h>

namespace engine {
  
ncurses_backend::ncurses_backend()
{
  initialized = false;
}

ncurses_backend::~ncurses_backend()
{
  if (initialized) {
    endwin();
  }
}
  
void ncurses_backend::init()
{
  initialized = true;
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();
}

pair< int, int > ncurses_backend::get_console_size()
{
  int rows, cols;
  getmaxyx(stdscr,rows,cols);
  return std::make_pair(cols,rows);
}

  
}

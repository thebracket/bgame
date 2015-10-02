#include "ncurses_backend.h"
#include <ncurses.h>
#include <iostream>
#include <cmath>

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
    supports_color = has_colors();
    if (supports_color) {
      start_color();
      number_of_colors = COLORS;
      number_of_color_pairs = COLOR_PAIRS;
      setup_colors();
    } else {
      number_of_colors = 0;
    }
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
	    auto fg = screen->operator[]((y*screen_width)+x).foreground_color;
	    auto bg = screen->operator[]((y*screen_width)+x).background_color;
	    
	    short pair_idx = 0;
	    if (supports_color) {
	      const short foreground_color_idx = map_color(fg);
	      const short background_color_idx = map_color(bg);
	      pair_idx = (background_color_idx * 8) + foreground_color_idx;
	    
	      attron(COLOR_PAIR(pair_idx));
	    }
            mvaddch(y, x, screen->operator[]((y*screen_width)+x).character);
	    if (supports_color) attroff(COLOR_PAIR(pair_idx));
        }
    }
    refresh();
}

void ncurses_backend::setup_colors()
{
  if (!supports_color) return;
  if (number_of_color_pairs == 64 && number_of_colors == 8) {
    //std::cout << "Number of colors: " << number_of_colors << ", in " << number_of_color_pairs << " pairs.\n";

    // We're on a default 8-color terminal, so we just need to map the default colors.
    //use_default_colors();
    
    // Actually setup the pairs
    for (short i=0; i<8; ++i) {
      for (short j=0; j<8; ++j) {
	init_pair((i*8)+j, j, i);
      }
    }
    
    for (short i=0; i<8; ++i) {
      short r,g,b;
      color_content(i, &r, &g, &b); // gets the current color
      //std::cout << i << " : " << r << ", " << g << ", " << b << "\n";
      color_map.push_back({i,r,g,b});
    }

  } else {
    // TODO: Add support for higher color terminals!
  }
}

short int ncurses_backend::map_color ( const tuple< unsigned char, unsigned char, unsigned char > &color )
{
    const tuple<short, short, short> color_fix { std::get<0>(color)*3.92, std::get<1>(color)*3.92, std::get<2>(color)*3.92  };
  
    // Treats this as a distance problem in an imaginary 3D space.
    short nearest = 0;
    double distance = 650000;
    
    for (ncurses::color_mapping map : color_map) {
      
      double n_distance = sqrt(
	(abs(std::get<0>(color_fix) - map.red)*abs(std::get<0>(color_fix) - map.red)) +
	(abs(std::get<1>(color_fix) - map.green)*abs(std::get<1>(color_fix) - map.green)) +
	(abs(std::get<2>(color_fix) - map.blue)*abs(std::get<2>(color_fix) - map.blue))
      );
      if (n_distance < distance) {
	//if (std::get<0>(color_fix) != 0) {
	//  std::cerr << "Distance: " << n_distance << ", selecting idx " << map.index << "\n";
	//}
	distance = n_distance;
	nearest = map.index;
      }
    }
    return nearest;
}

}

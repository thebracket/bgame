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
        std::cout << "NCurses mode: " << COLORS << "x" << COLOR_PAIRS << "\n";
    } else {
        number_of_colors = 0;
    }
    raw();
    nonl();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    cbreak();
    nodelay(stdscr, TRUE);
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
                if (std::get<0>(fg)>128 or std::get<1>(fg)>128 or std::get<2>(fg)>128) {
                    attron(A_BOLD);
                }
            }
            int base_ascii = screen->operator[]((y*screen_width)+x).character;
            if (base_ascii > 127) base_ascii = extended_map(base_ascii);
            mvaddch(y, x, base_ascii );
            if (supports_color) {
                attroff(COLOR_PAIR(pair_idx));
                if (std::get<0>(fg)>128 or std::get<1>(fg)>128 or std::get<2>(fg)>128) {
                    attroff(A_BOLD);
                }
            }
        }
    }
    refresh();
}

int ncurses_backend::extended_map(const int ascii)
{
    switch (ascii) {
    case 96  :
        return ACS_DIAMOND;
    case 97  :
        return ACS_CKBOARD;
    case 102 :
        return ACS_DEGREE;
    case 104 :
        return ACS_BLOCK;
    case 125 :
        return ACS_STERLING;
    case 126 :
        return ACS_BULLET;
    case 177 :
        return ACS_PLMINUS;
    case 179 :
        return ACS_VLINE;
    case 191 :
        return ACS_URCORNER;
    case 192 :
        return ACS_LLCORNER;
    case 196 :
        return ACS_HLINE;
    case 217 :
        return ACS_LRCORNER;
    case 218 :
        return ACS_ULCORNER;
    }
    return NCURSES_ACS(ascii);
}

void ncurses_backend::setup_default_colors() {
    for (short i=0; i<8; ++i) {
        for (short j=0; j<8; ++j) {
            init_pair((i*8)+j, j, i);
        }
    }
}

void ncurses_backend::populate_color_map() {
    for (short i=0; i<number_of_colors; ++i) {
        short r,g,b;
        color_content(i, &r, &g, &b); // gets the current color
        //std::cout << i << " : " << r << ", " << g << ", " << b << "\n";
        color_map.push_back( {i,r,g,b});
    }
}

void ncurses_backend::setup_colors()
{
    if (!supports_color or number_of_colors<8) return;
    if (number_of_color_pairs == 64 && number_of_colors == 8) {
        //std::cout << "Number of colors: " << number_of_colors << ", in " << number_of_color_pairs << " pairs.\n";

        // We're on a default 8-color terminal, so we just need to map the default colors.
        //use_default_colors();

        // Actually setup the pairs
        setup_default_colors();
        populate_color_map();

    } else {
        // Set the first 8 as normal
        setup_default_colors();
        // We'll do something clever here later!
        populate_color_map();
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

command::keys ncurses_backend::translate_keycode(const int &key) {
  //std::cout << key << "\n";
  switch (key) {
    case KEY_UP : return command::UP;
    case KEY_DOWN : return command::DOWN;
    case KEY_LEFT : return command::LEFT;
    case KEY_RIGHT : return command::RIGHT;
    case KEY_ENTER : return command::ENTER;
    case 13 : return command::ENTER;
    case 'Q' : return command::Q;
    case 'q' : return command::Q;
  }
  return command::NONE;
}

void ncurses_backend::poll_inputs()
{
    int ch = getch();
    if (ch != ERR) {
        // We have input to read
        command::on_command( {translate_keycode(ch), 0, 0, command::KEYDOWN});
    }
}


}

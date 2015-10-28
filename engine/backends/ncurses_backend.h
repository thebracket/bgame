#pragma once

#include "command_manager.h"
#include "../virtual_terminal.h"
#include <vector>
#include <utility>

using std::pair;
using std::vector;
using std::tuple;

namespace engine {

namespace ncurses {

struct color_mapping {
    short index;
    short red;
    short green;
    short blue;
};

}

class ncurses_backend {
public:
    ncurses_backend();
    ~ncurses_backend();
    void init();
    void stop();
    pair<int,int> get_console_size();
    void draw(vector<vterm::screen_character>* screen);
    void poll_inputs();

private:
    void setup_colors();
    short map_color(const tuple<unsigned char, unsigned char, unsigned char> &color);

    bool initialized;
    bool supports_color;
    int number_of_color_pairs;
    int number_of_colors;
    int screen_height;
    int screen_width;

    vector<ncurses::color_mapping> color_map;
    void populate_color_map();
    void setup_default_colors();
    command::keys translate_keycode(const int &key);
};

}
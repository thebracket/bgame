#pragma once

#include "command_manager.h"
#include "../virtual_terminal.h"
#include <vector>
#include <utility>

using std::pair;
using std::vector;
using std::tuple;

namespace engine {

class ansi_backend {
public:
     ansi_backend();
     ~ansi_backend();
     void init();
     void stop();
     pair<int,int> get_console_size();
     void draw ( vector<vterm::screen_character>* screen );
     void poll_inputs();

private:
    int screen_width;
    int screen_height;
    command::keys translate_keycode(const int &key);
};

}

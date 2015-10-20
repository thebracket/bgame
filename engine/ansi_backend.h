#pragma once

#include "output_backend.h"
#include "command_manager.h"
#include <vector>
#include <utility>

using std::pair;
using std::vector;
using std::tuple;

namespace engine {

class ansi_backend : public output_backend {
public:
     ansi_backend();
     ~ansi_backend();
     virtual void init();
     virtual void stop();
     virtual pair<int,int> get_console_size();
     virtual void draw ( vector<vterm::screen_character>* screen );
     virtual void poll_inputs();

private:
    int screen_width;
    int screen_height;
    command::keys translate_keycode(const int &key);
};

}

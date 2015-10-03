#pragma once

#include "virtual_terminal.h"
#include <utility>
#include <vector>

using std::pair;
using std::vector;

namespace engine {

/* Abstract class that provides a starting-point for actual
 * back-end implementations, such as SDL and Curses.
 */
class output_backend {
public:
    virtual void init()=0;
    virtual void stop()=0;
    virtual pair<int,int> get_console_size()=0;
    virtual void poll_inputs()=0;
    virtual void draw(vector<vterm::screen_character>* screen)=0;
};

}

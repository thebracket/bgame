#pragma once

#include "../virtual_terminal.h"
#include <utility>
#include <vector>
#include <memory>

using std::pair;
using std::vector;
using std::unique_ptr;

namespace engine {

enum backend_mode { NCURSES, SDL2, ANSI };
  
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

unique_ptr<output_backend> init_backend(const backend_mode &mode);


}

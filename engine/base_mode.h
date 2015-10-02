#pragma once

#include <memory>
#include <utility>

using std::unique_ptr;
using std::pair;

namespace engine {

enum return_mode { CONTINUE, PUSH, POP };

/*
 * Basic game mode to inherit from, for the mode stack system.
 */
class base_mode {
public:
     virtual void init() =0;
     virtual void done() =0;
     virtual pair<return_mode, unique_ptr<base_mode>> tick ( const double time_elapsed ) =0;
     virtual void on_push() {};
     virtual void on_pop() {};
};

#define NO_PUSHED_MODE unique_ptr<base_mode>()

}

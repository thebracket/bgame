#pragma once

#include "../engine/engine.h"

using std::make_pair;
using engine::vterm::color_t;

class loading_screen : public engine::base_mode {
public:
     virtual void init();
     virtual void done();
     virtual pair<engine::return_mode, unique_ptr<base_mode>> tick ( const double time_elapsed );
private:
    bool loaded_assets;
    bool displayed;
    engine::gui splash_interface;
    int count;
};

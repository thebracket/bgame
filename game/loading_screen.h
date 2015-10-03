#pragma once

#include "../engine/engine.h"

using namespace engine;
using std::make_pair;
using engine::vterm::color_t;

class loading_screen : public engine::base_mode {
public:
     virtual void init();
     virtual void done();
     virtual pair<return_mode, unique_ptr<base_mode>> tick ( const double time_elapsed );
private:
    bool loaded_assets;
    bool displayed;
    gui splash_interface;
    int count;
};

#pragma once

#include "../engine/engine.h"

using namespace engine;
using std::make_pair;
using engine::vterm::color_t;

class play_mode : public engine::base_mode {
public:
     virtual void init();
     virtual void done();
     virtual pair<return_mode, unique_ptr<base_mode>> tick ( const double time_elapsed );
private:
    gui ui;
    bool quitting;
    
    void init_gui();
};

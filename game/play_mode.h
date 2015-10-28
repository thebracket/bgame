#pragma once

#include "../engine/engine.h"

using std::make_pair;
using engine::vterm::color_t;

using namespace engine::ecs;

class play_mode : public engine::base_mode {
public:
     virtual void init();
     virtual void done();
     virtual pair<engine::return_mode, unique_ptr<base_mode>> tick ( const double time_elapsed );
private:
    engine::gui ui;
    bool quitting;            
    void init_gui();
};

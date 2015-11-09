#pragma once

#include "../engine/engine.h"

using std::make_pair;
using engine::vterm::color_t;

class play_mode : public engine::base_mode {
public:
     virtual void init();
     virtual void done();
     virtual pair<engine::return_mode, unique_ptr<base_mode>> tick ( const double time_elapsed );
     virtual engine::scene_graph * get_render_target() { return &sg; }
private:
    bool quitting;            
    void init_systems();
    engine::scene_graph sg;
};

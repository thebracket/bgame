#pragma once

#include "../engine/engine.h"

class loading_screen : public engine::base_mode {
public:
     virtual void init();
     virtual void done();
     virtual pair<engine::return_mode, unique_ptr<base_mode>> tick ( const double time_elapsed );
     virtual engine::scene_graph * get_render_target() { return &sg; }
private:
    bool loaded_assets;
    bool displayed;
    int count;
    engine::scene_graph sg;
};

#pragma once

#include "../engine/engine.h"

class loading_screen : public engine::base_mode {
public:
     virtual void init() override;
     virtual void done() override;
     virtual pair<engine::return_mode, unique_ptr<base_mode>> tick ( const double time_elapsed ) override;
     virtual engine::scene_graph * get_render_target() override { return &sg; }
private:
    bool loaded_assets;
    bool displayed;
    int count;
    engine::scene_graph sg;
};

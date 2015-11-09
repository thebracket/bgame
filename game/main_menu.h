#pragma once

#include "../engine/engine.h"

using std::make_pair;
using engine::vterm::color_t;

class main_menu : public engine::base_mode {
public:
     virtual void init();
     virtual void done();
     virtual pair<engine::return_mode, unique_ptr<base_mode>> tick ( const double time_elapsed );
     virtual void on_pop();
     virtual engine::scene_graph * get_render_target() { return &sg; }
private:
    int selected_item;
    bool loaded_assets;
    bool displayed;
    bool world_available;
    engine::scene_graph sg;
};

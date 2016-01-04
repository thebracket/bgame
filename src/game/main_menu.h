#pragma once

#include "../engine/engine.hpp"

using std::make_pair;
using engine::vterm::color_t;

class main_menu : public engine::base_mode {
public:
     virtual void init() override;
     virtual void done() override;
     virtual pair<engine::return_mode, unique_ptr<base_mode>> tick ( const double time_elapsed ) override;
     virtual void on_pop() override;
     virtual engine::scene_graph * get_render_target() override { return &sg; }
private:
    int selected_item;
    bool loaded_assets;
    bool displayed;
    bool world_available;
    engine::scene_graph sg;
};

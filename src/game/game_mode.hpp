#pragma once

#include <memory>
#include "../engine/engine.hpp"

class game_mode : public engine::base_mode {
public:
     virtual void init() override;
     virtual void done() override;
     virtual pair<engine::return_mode, unique_ptr<base_mode>> tick ( const double time_elapsed ) override;
     virtual engine::scene_graph * get_render_target() override { return &sg; }
private:
     bool finished;
     
     engine::scene_graph sg;
     
     void init_systems();          
     
};

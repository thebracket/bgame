#pragma once

#include "../engine/engine.h"

enum genesis_stage_t { INIT, UGEN, UDISPLAY };

class genesis_mode : public engine::base_mode {
public:
     virtual void init() override;
     virtual void done() override;
     virtual pair<engine::return_mode, unique_ptr<base_mode>> tick ( const double time_elapsed ) override;
private:
    bool finished = false;
    genesis_stage_t mode;
    
    void begin_genesis();
    void ugen();
    void udisplay();
    
    void show_universe_map();
};

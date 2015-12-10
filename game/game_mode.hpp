#pragma once

#include "../engine/engine.h"

class game_mode : public engine::base_mode {
public:
     virtual void init() override;
     virtual void done() override;
     virtual pair<engine::return_mode, unique_ptr<base_mode>> tick ( const double time_elapsed ) override;
private:
     bool finished;
     
     uint8_t x,y,z;
};

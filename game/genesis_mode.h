#pragma once

#include "../engine/engine.h"
#include "world/universegen.hpp"

enum genesis_stage_t { INIT, UGEN, UDISPLAY, UDISPLAY2, TRAVEL };

class genesis_mode : public engine::base_mode {
public:
     virtual void init() override;
     virtual void done() override;
     virtual pair<engine::return_mode, unique_ptr<base_mode>> tick ( const double time_elapsed ) override;
private:
    bool finished = false;
    genesis_stage_t mode;
    universe_generator unigen;
    bool spread_humanity = false;
    int starting_x = 0;
    int starting_y = 0;
    int current_x;
    int current_y;
    std::vector<std::pair<int,int>> flight_path;
    int flight_progress = 0;
    
    void begin_genesis( const double time_elapsed );
    void ugen( const double time_elapsed );
    void udisplay( const double time_elapsed );
    void udisplay2( const double time_elapsed );
    void travel( const double time_elapsed );
    
    void show_universe_map( const bool show_warzone, const bool show_truce=false );
    void show_static_map();
    void intro_quote();
    
    double total_time;
};

#pragma once

#include "../engine/engine.h"

using namespace engine;
using std::make_pair;
using engine::vterm::color_t;

class main_menu : public engine::base_mode {
public:
     virtual void init();
     virtual void done();
     virtual pair<return_mode, unique_ptr<base_mode>> tick ( const double time_elapsed );
private:
    int selected_item;
    bool loaded_assets;
    bool displayed;
    bool world_available;
    gui menu_interface;
};

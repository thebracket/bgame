#pragma once

#include "../virtual_terminal.h"
#include "gui_element_base.h"
#include "gui_static_text.h"
#include "gui_frame.h"
#include "gui_console.h"
#include "gui_main_game_view.h"
#include "gui_main_game_command_panel.h"
#include <vector>
#include <memory>
#include <string>

using std::vector;
using std::unique_ptr;
using std::string;

/* Master header to include the rest */

namespace engine {

class gui {
public:
    gui();
    
    std::pair<int,int> get_screen_size() {
        return std::make_pair(screen_width, screen_height);
    };
    
    void add_child(unique_ptr<gui_element_base> child) {
        children.push_back(std::move(child));
    };
    gui_element_base * find_child(string tag);
    
    void render();
private:
    int screen_width;
    int screen_height;
    vector<unique_ptr<gui_element_base>> children;
};

}

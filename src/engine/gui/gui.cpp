#include "gui.h"
#include <utility>

namespace engine {

gui::gui()
{
    auto screen_dimensions = vterm::get_screen_size();
    screen_width = screen_dimensions.first;
    screen_height = screen_dimensions.second;
}

void gui::render()
{
    const std::pair<int,int> screen_size = get_screen_size();
    const screen_region viewport {
        0,0,screen_size.first-1, screen_size.second-1
    };
    for (const auto &element : children) {
        element->render(viewport);
    }
}

gui_element_base* gui::find_child(string tag)
{
    gui_element_base * result = nullptr;
    for (const auto &element : children) {
        result = element->find(tag);
        if (result != nullptr) return result;
    }
    return nullptr;
}


}

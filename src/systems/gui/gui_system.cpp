#include "gui_system.hpp"

std::vector<std::unique_ptr<base_gui>> gui_elements;

void gui_system::configure() {
    system_name = "GUI System";
}

void gui_system::update(const double ms) {
    for (auto &g : gui_elements) {
        g->render();
    }
    gui_elements.clear();
}

#pragma once

#include "gl_include.hpp"
#include <functional>

namespace bengine {
    extern GLFWwindow * main_window;

    extern std::function<void(const double &)> main_func;

    void init(bool fullscreen, int width, int height, std::string gui_font, int gui_font_size);
    void run();
}
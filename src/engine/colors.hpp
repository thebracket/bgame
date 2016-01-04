#pragma once

#include "virtual_terminal.hpp"

using engine::vterm::color_t;

namespace engine {
  
constexpr color_t dark_red {128,0,0};
constexpr color_t red {255,0,0};
constexpr color_t green {0,255,0};
constexpr color_t dark_green {0,128,0};
constexpr color_t blue {0,0,255};
constexpr color_t dark_blue {0,0,128};
constexpr color_t white {255,255,255};
constexpr color_t dark_grey {64,64,64};
constexpr color_t grey {128,128,128};
constexpr color_t black {0,0,0};
constexpr color_t dark_yellow {128,128,0};
constexpr color_t yellow {255,255,0};
constexpr color_t dark_magenta {128,0,128};
constexpr color_t magenta {255,0,255};
constexpr color_t dark_cyan {0,128,128};
constexpr color_t cyan {0,255,255};
constexpr color_t orange {255,165,0};
constexpr color_t brown {165,42,42};

}
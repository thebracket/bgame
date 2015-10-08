#pragma once

#include <utility>
#include <vector>
#include <memory>
#include <string>
#include "../virtual_terminal.h"

using std::vector;
using std::unique_ptr;
using std::string;
using engine::vterm::color_t;

namespace engine {

struct screen_region {
    int x;
    int y;
    int w;
    int h;
};

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

class gui_element_base {
public:
    virtual void render(const screen_region viewport)=0;

    virtual void add_child(unique_ptr<gui_element_base> child) {
        children.push_back(std::move(child));
    }
    
    gui_element_base * find(const string &name) {
      if (tag == name) return this;
      for (const auto& child : children) {
	gui_element_base *r = child->find(name);
	if (r != nullptr) return r;
      }
      return nullptr;
    }

    string tag;

    vector<unique_ptr<gui_element_base>> children;
};

}

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

constexpr color_t red {255,0,0};
constexpr color_t white {255,255,255};
constexpr color_t grey {128,128,128};
constexpr color_t black {0,0,0};

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

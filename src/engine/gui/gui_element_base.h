#pragma once

#include <utility>
#include <vector>
#include <memory>
#include <string>
#include "../colors.hpp"
#include "../virtual_terminal.hpp"

using std::vector;
using std::unique_ptr;
using std::string;

namespace engine {

struct screen_region {
    int x;
    int y;
    int w;
    int h;
};

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

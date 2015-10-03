#pragma once

#include "output_backend.h"
#include <vector>
#include <utility>

using std::pair;
using std::vector;
using std::tuple;

namespace engine {

namespace ncurses {
  
struct color_mapping {
  short index;
  short red;
  short green;
  short blue;
};
  
}
  
class ncurses_backend : public output_backend {
public:
  ncurses_backend();
  ~ncurses_backend();
  virtual void init();
  virtual void stop();
  virtual pair<int,int> get_console_size();
  virtual void draw(vector<vterm::screen_character>* screen);
  
private:
  void setup_colors();
  short map_color(const tuple<unsigned char, unsigned char, unsigned char> &color);
  
  bool initialized;
  bool supports_color;
  int number_of_color_pairs;
  int number_of_colors;
  int screen_height;
  int screen_width;
  
  vector<ncurses::color_mapping> color_map;
  int extended_map(const int ascii);
  void populate_color_map();
  void setup_default_colors();
};
  
}
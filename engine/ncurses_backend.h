#pragma once

#include "output_backend.h"

using std::pair;

namespace engine {

class ncurses_backend : public output_backend {
public:
  ncurses_backend();
  ~ncurses_backend();
  virtual void init();
  virtual void stop();
  virtual pair<int,int> get_console_size();
  virtual void draw(vector<vterm::screen_character>* screen);
  
private:
  bool initialized;
  int screen_height;
  int screen_width;
};
  
}
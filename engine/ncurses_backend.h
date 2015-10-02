#pragma once

#include "output_backend.h"

using std::pair;

namespace engine {

class ncurses_backend : public output_backend {
public:
  ncurses_backend();
  ~ncurses_backend();
  void init();
  pair<int,int> get_console_size();
  
private:
  bool initialized;
};
  
}
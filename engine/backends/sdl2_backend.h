#pragma once

#include "output_backend.h"
#include <SDL2/SDL.h>

using std::pair;

namespace engine {

class sdl2_backend : public output_backend {
public:
  sdl2_backend();
  ~sdl2_backend();
  virtual void init();
  virtual void stop();
  virtual pair<int,int> get_console_size();
  virtual void draw(vector<vterm::screen_character>* screen);
  virtual void poll_inputs();
private:
  bool initialized;
  SDL_Window * window;
  SDL_Renderer * renderer;
  SDL_Texture * font_image;
};
  
}
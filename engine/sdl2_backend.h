#pragma once

#include "virtual_terminal.h"
#include "sdl2_resources.h"
#include <SDL2/SDL.h>
#include <string>

using std::pair;

namespace engine {

class sdl2_backend {
public:
  sdl2_backend();
  ~sdl2_backend();
  void init(const string& window_title, const int width, const int height);
  void stop();
  pair<int,int> get_console_size();
  void draw(vector<vterm::screen_character>* screen);
  void poll_inputs();
  
  int load_image_resource(const std::string &filename, const std::string &tag);
private:
  bool initialized;
  SDL_Window * window;
  SDL_Renderer * renderer;
  
  sdl_resource_manager resources;  
};
  
}
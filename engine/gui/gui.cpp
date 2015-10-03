#include "gui.h"
#include <utility>

namespace engine {
 
gui::gui()
{
  auto screen_dimensions = vterm::get_screen_size();
  screen_width = screen_dimensions.first;
  screen_height = screen_dimensions.second;
}

void gui::render()
{
  const std::pair<int,int> screen_size = get_screen_size();
  for (const auto &element : children)
    element->render(screen_size);
}


  
}

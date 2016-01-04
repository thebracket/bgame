#include "command_manager.hpp"

#include <set>

using std::set;

namespace engine {
namespace command {

std::set<keys> key_down;
int mouse_x = 0;
int mouse_y = 0;
bool left_click = false;
bool right_click = false;
  
void clear_commands()
{
  key_down.clear();
  left_click = false;
  right_click = false;
}
  
void on_command(const input_data &command)
{
  if (command.type == KEYDOWN && command.key != NONE) {
      key_down.insert(command.key);
      return;
  }
  if (command.type == MOUSE_MOVE) {
      mouse_x = command.x;
      mouse_y = command.y;
      return;
  }
  if (command.type == MOUSE_LEFT_CLICK) {
      left_click = true;
  }
  if (command.type == MOUSE_RIGHT_CLICK) {
      right_click = true;
  }
}

bool is_key_down(const keys key) {
    auto finder = key_down.find(key);
    if (finder == key_down.end()) return false;
    return true;
}
  
}
}

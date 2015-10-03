#include "command_manager.h"
#include <set>

using std::set;

namespace engine {
namespace command {

std::set<keys> key_down;
  
void clear_commands()
{
  key_down.clear();
}
  
void on_command(const input_data &command)
{
  if (command.type == KEYDOWN && command.key != NONE) key_down.insert(command.key);
}

void process_commands()
{

}

bool is_key_down(const keys key) {
    auto finder = key_down.find(key);
    if (finder == key_down.end()) return false;
    return true;
}
  
}
}

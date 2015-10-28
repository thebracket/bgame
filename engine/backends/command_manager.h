#pragma once

namespace engine {
namespace command {

enum command_type {MOUSE_MOVE, MOUSE_LEFT_CLICK, MOUSE_RIGHT_CLICK, KEYDOWN};
enum keys { NONE, UP, DOWN, LEFT, RIGHT, ENTER, Q };
  
struct input_data {
  keys key;
  int x;
  int y;
  command_type type;
};

/*
 * Clear the command queue; you want to do this before you
 * poll inputs.
 */
void clear_commands();

/*
 * Register a command from one of the back-ends
 */
void on_command(const input_data &command);


/*
 * Perform any command processing required before the
 * tick process receives input.
 */
void process_commands();

/*
 * Is a key pressed?
 */
bool is_key_down(const keys key);

}
 
}

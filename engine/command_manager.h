#pragma once

namespace engine {
namespace command {

enum command_type {MOUSE_MOVE, MOUSE_LEFT_CLICK, MOUSE_RIGHT_CLICK, KEYDOWN};
enum keys { NONE, SPACE, UP, DOWN, LEFT, RIGHT, COMMA, PERIOD, ENTER, P, Q, TILDE, PRINTSCREEN };
  
struct input_data {
  keys key;
  int x;
  int y;
  command_type type;
};

extern int mouse_x;
extern int mouse_y;
extern bool left_click;
extern bool right_click;

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
 * Is a key pressed?
 */
bool is_key_down(const keys key);

}
 
}

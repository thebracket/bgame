#pragma once

struct pause_requested_message : public rltk::base_message_t {};
struct one_step_requested_message : public rltk::base_message_t {};
struct camera_move_requested_message : public rltk::base_message_t { 
    int direction; int step;
    camera_move_requested_message() {}
    camera_move_requested_message(int dir, int n) : direction(dir), step(n) {} 
};

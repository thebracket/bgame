#pragma once

#include <string>
#include "../render/panel.h"

using std::string;

enum chat_emote_color_t {WHITE, YELLOW, CYAN, GREEN, MAGENTA, RED, BLUE, BLACK};

struct chat_emote_message {  
    chat_emote_message() { message = ""; }
    chat_emote_message( const string &msg, const int &X, const int &Y, const chat_emote_color_t &col = YELLOW ) : message(msg), x(X), y(Y), color(col) {}
    ~chat_emote_message() {
      if (render_details != nullptr) delete render_details;
    }
  
    bool deleted = false;
    int ttl = 32;
    
    string message;
    int x;
    int y;
    chat_emote_color_t color;
    render::panel * render_details = nullptr;
};
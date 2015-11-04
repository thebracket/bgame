#pragma once

#include <string>

using std::string;

enum chat_emote_color_t {WHITE, YELLOW, CYAN, GREEN, MAGENTA, RED};

struct chat_emote_message {  
    chat_emote_message() { message = ""; }
    chat_emote_message( const string &msg, const int &X, const int &Y, const chat_emote_color_t &col = YELLOW ) : message(msg), x(X), y(Y) {}
  
    bool deleted = false;
    int ttl = 64;
    
    string message;
    int x;
    int y;
    chat_emote_color_t color;
};
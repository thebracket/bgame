#pragma once

#include <string>

using std::string;

struct chat_emote_message {  
    chat_emote_message() { message = ""; }
    chat_emote_message( const string &msg, const int &X, const int &Y ) : message(msg), x(X), y(Y) {}
  
    bool deleted = false;
    int ttl = 32;
    
    string message;
    int x;
    int y;
};
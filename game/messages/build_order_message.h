#pragma once

#include <string>

using std::string;

struct build_order_message {
    build_order_message() {  }
    build_order_message(const int &X, const int &Y, const string &name) : x(X), y(Y), structure_name(name) {}
  
    bool deleted = false;
    int ttl = 2;
    
    int x;
    int y;
    int z;
    string structure_name;
};
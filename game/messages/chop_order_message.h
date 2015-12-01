#pragma once

#include <string>

using std::string;

struct chop_order_message {
    chop_order_message() {  }
    chop_order_message(const int &X, const int &Y, const int &eid, const string &name) : x(X), y(Y), tree_id(eid), structure_name(name) {}
  
    bool deleted = false;
    int ttl = 2;
    
    int x;
    int y;
    int tree_id;
    string structure_name;
};
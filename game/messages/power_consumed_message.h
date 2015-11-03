#pragma once

struct power_consumed_message {
    power_consumed_message() { quantity = 0; }
    power_consumed_message(const int &qty) : quantity(qty) {}
  
    bool deleted = false;
    int ttl = 0;
  
    int quantity;
};
#pragma once

#include <rltk.hpp>

struct power_consumed_message : public rltk::base_message_t {
    power_consumed_message() {}
    power_consumed_message(const int &amt) : amount(amt) {}

    int amount = 0;
};
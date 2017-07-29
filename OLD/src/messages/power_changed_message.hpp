#pragma once

#include <rltk.hpp>

struct power_changed_message : public rltk::base_message_t {
    power_changed_message() {}
    power_changed_message(const int &amt) : amount(amt) {}

    int amount = 0;
};
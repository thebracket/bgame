#pragma once

#include <rltk.hpp>

struct perform_mining_message : public rltk::base_message_t {
    perform_mining_message() {}
    perform_mining_message(const int idx, const uint8_t op, int X, int Y, int Z) : target_idx(idx),
        operation(op), x(X), y(Y), z(Z) {}
    int target_idx;
    uint8_t operation;
    int x,y,z;
};

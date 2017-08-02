#pragma once

#include <rltk.hpp>

struct vegetation_damage_message : public rltk::base_message_t {
    vegetation_damage_message() {}
    vegetation_damage_message(const int &IDX, const int &dmg) : idx(IDX), damage(dmg) {}
    int idx;
    int damage;
};

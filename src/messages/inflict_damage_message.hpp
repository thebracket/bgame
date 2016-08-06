#pragma once

#include <rltk.hpp>

struct inflict_damage_message : public rltk::base_message_t {
	inflict_damage_message() {}
    inflict_damage_message(std::size_t target, int amount, std::string type) : victim(target), damage_amount(amount), damage_type(type) {}

    std::size_t victim;
    int damage_amount;
    std::string damage_type;
};

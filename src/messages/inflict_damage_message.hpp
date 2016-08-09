#pragma once

#include <rltk.hpp>

struct inflict_damage_message : public rltk::base_message_t {
	inflict_damage_message() {}
    inflict_damage_message(std::size_t target, int amount, std::string type) : victim(target), damage_amount(amount), damage_type(type) {}

    std::size_t victim;
    int damage_amount;
    std::string damage_type;
};

struct creature_attack_message : public rltk::base_message_t {
    creature_attack_message() {}
    creature_attack_message(const std::size_t attack, const std::size_t defend) : attacker(attack), victim(defend) {}

    std::size_t attacker;
    std::size_t victim;
};

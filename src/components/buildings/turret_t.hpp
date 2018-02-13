#pragma once

struct turret_t {
	turret_t() = default;
    turret_t(const int rng, const int hb, const int n, const int d, const int bonus, const std::size_t civ) noexcept 
	: range(rng), hit_bonus(hb), damage_dice(n), damage_die(d), damage_bonus(bonus), owner_civilization(civ) {}

    int range = 6;
    int hit_bonus = 0;
    int damage_dice = 1;
    int damage_die = 6;
    int damage_bonus = 0;
    std::size_t owner_civilization = 0;
};

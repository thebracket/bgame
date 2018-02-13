#pragma once

struct initiative_t {
	initiative_t() = default;
    explicit initiative_t(const int init_mod) noexcept : initiative_modifier(init_mod) {}
    int initiative = 0;
    int initiative_modifier = 0;
};

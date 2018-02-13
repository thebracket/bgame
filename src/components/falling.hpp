#pragma once

struct falling_t {
	falling_t() = default;
    explicit falling_t(const int &d) noexcept : distance(d) {}
    int distance = 0;
};


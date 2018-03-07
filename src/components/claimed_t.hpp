#pragma once

#include <string>

struct claimed_t {

	claimed_t() = default;
    explicit claimed_t(const int &id) noexcept : claimed_by(id) {}
    int claimed_by = 0;

};


#pragma once

#include <string>

struct claimed_t {

	claimed_t() = default;
    explicit claimed_t(const std::size_t &id) noexcept : claimed_by(id) {}
    std::size_t claimed_by = 0;

};


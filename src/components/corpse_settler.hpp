#pragma once

#include <string>

struct corpse_settler {
	corpse_settler() = default;
	explicit corpse_settler(const std::string cause) noexcept : cause_of_death(cause) {}
    uint32_t ticks_since_death = 0;
	std::string cause_of_death = "";	
};


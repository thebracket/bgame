#pragma once

#include <istream>
#include <ostream>
#include <cereal/cereal.hpp>

struct skill_t {
	skill_t() = default;
	skill_t(const int8_t lvl, const uint16_t &xp) noexcept : skill_level(lvl), experience_gained(xp) {}
	int16_t skill_level = 0;
	uint16_t experience_gained = 0;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( skill_level, experience_gained ); // serialize things by passing them to the archive
    }
};
#pragma once

#include <cstdint>

struct wildlife_group {
	wildlife_group() = default;
	explicit wildlife_group(uint8_t &id) noexcept : group_id(id) {}
    uint8_t group_id;	
};


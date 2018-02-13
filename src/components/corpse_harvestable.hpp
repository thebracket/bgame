#pragma once

#include <string>

struct corpse_harvestable {
	corpse_harvestable() = default;
    explicit corpse_harvestable(const std::string &tag) noexcept : creature_tag(tag) {}
    uint32_t ticks_since_death = 0;
    std::string creature_tag = "";
	bool claimed = false;

};

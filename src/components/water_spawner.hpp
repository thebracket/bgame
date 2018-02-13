#pragma once

struct water_spawner_t {
	water_spawner_t() = default;
	explicit water_spawner_t(const uint8_t &t) noexcept : spawner_type(t) {}

    uint8_t spawner_type = 0;	
};

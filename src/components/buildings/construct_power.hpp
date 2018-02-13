#pragma once

struct construct_power_t {
	int storage_capacity;
	int generation_always;
	int generation_solar;

	construct_power_t() = default;
	construct_power_t(const int store, const int gen_all, const int gen_solar) : storage_capacity(store), generation_always(gen_all), generation_solar(gen_solar) {}

};


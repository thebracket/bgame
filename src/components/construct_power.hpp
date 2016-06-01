#pragma once

#include <rltk.hpp>

using namespace rltk;

struct construct_power_t {
	int storage_capacity;
	int generation_always;
	int generation_solar;

	construct_power_t() {}
	construct_power_t(const int store, const int gen_all, const int gen_solar) : storage_capacity(store), generation_always(gen_all), generation_solar(gen_solar) {}

	std::size_t serialization_identity = 15;

	void save(std::ostream &lbfile) {
		serialize(lbfile, storage_capacity);
		serialize(lbfile, generation_always);
		serialize(lbfile, generation_solar);
	}

	static construct_power_t load(std::istream &lbfile) {
		construct_power_t c;
		deserialize(lbfile, c.storage_capacity);
		deserialize(lbfile, c.generation_always);
		deserialize(lbfile, c.generation_solar);
		return c;
	}
};
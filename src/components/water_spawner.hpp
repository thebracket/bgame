#pragma once

#include <rltk.hpp>

using namespace rltk;

struct water_spawner_t {
	water_spawner_t() {}
	water_spawner_t(const uint8_t &t) : spawner_type(t) {}

    uint8_t spawner_type = 0;

	std::size_t serialization_identity = 27;

	void save(std::ostream &lbfile);
	static water_spawner_t load(std::istream &lbfile);
};
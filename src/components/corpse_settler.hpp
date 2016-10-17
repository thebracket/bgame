#pragma once

#include <rltk.hpp>

using namespace rltk;

struct corpse_settler {
	corpse_settler() {}
	corpse_settler(const std::string cause) : cause_of_death(cause) {}
    uint32_t ticks_since_death = 0;
	std::string cause_of_death = "";

	std::size_t serialization_identity = 22;

	void save(std::ostream &lbfile);
	static corpse_settler load(std::istream &lbfile);
};
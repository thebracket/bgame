#pragma once

#include <rltk.hpp>

using namespace rltk;

struct settler_ai_t {

	int initiative = 0;

	settler_ai_t() {}

	std::size_t serialization_identity = 11;

	void save(std::ostream &lbfile) {
		serialize(lbfile, initiative);
	}

	static settler_ai_t load(std::istream &lbfile) {
		settler_ai_t c;
		deserialize(lbfile, c.initiative);
		return c;
	}
};
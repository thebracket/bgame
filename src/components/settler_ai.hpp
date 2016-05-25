#pragma once

#include <rltk.hpp>

using namespace rltk;

struct settler_ai_t {

	settler_ai_t() {}

	std::size_t serialization_identity = 11;

	void save(std::ostream &lbfile) {
	}

	static settler_ai_t load(std::istream &lbfile) {
		settler_ai_t c;
		return c;
	}
};
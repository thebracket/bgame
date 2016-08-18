#pragma once

#include <rltk.hpp>

using namespace rltk;

struct smoke_emitter_t {
	smoke_emitter_t() {}

	std::size_t serialization_identity = 24;

	void save(std::ostream &lbfile) {
	}

	static smoke_emitter_t load(std::istream &lbfile) {
		smoke_emitter_t c;
		return c;
	}
};
#pragma once

#include <rltk.hpp>

using namespace rltk;

struct construct_container_t {
	construct_container_t() {}

	std::size_t serialization_identity = 14;

	void save(std::ostream &lbfile) {
	}

	static construct_container_t load(std::istream &lbfile) {
		construct_container_t c;
		return c;
	}
};
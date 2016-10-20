#pragma once

#include <rltk.hpp>

using namespace rltk;

struct falling_t {
	falling_t() {}
    falling_t(const int &d) : distance(d) {}
    int distance = 0;

	std::size_t serialization_identity = 28;

	void save(std::ostream &lbfile);
	static falling_t load(std::istream &lbfile);
};
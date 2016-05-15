#pragma once

#include <rltk.hpp>

using namespace rltk;

struct name_t {
	std::string first_name;
	std::string last_name;
	std::size_t serialization_identity = 5;

	name_t() {}
	name_t(const std::string fn, const std::string ln) : first_name(fn), last_name(ln) {}

	void save(std::ostream &lbfile) {
		serialize(lbfile, first_name);
		serialize(lbfile, last_name);
	}

	static name_t load(std::istream &lbfile) {
		name_t c;
		deserialize(lbfile, c.first_name);
		deserialize(lbfile, c.last_name);
		return c;
	}
};
#pragma once

#include <rltk.hpp>

using namespace rltk;

struct name_t {
	std::string first_name;
	std::string last_name;
	std::size_t serialization_identity = 5;

	name_t() {}
	name_t(const std::string fn, const std::string ln) : first_name(fn), last_name(ln) {}

	void save(std::ostream &lbfile);
	static name_t load(std::istream &lbfile);
};
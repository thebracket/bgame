#pragma once

#include <rltk.hpp>
#include <string>

using namespace rltk;

struct item_stored_t {
	std::size_t stored_in;
	item_stored_t() {}
	item_stored_t(const std::size_t carrier) : stored_in(carrier) {}

	std::size_t serialization_identity = 12;

	void save(std::ostream &lbfile);
	static item_stored_t load(std::istream &lbfile);
};
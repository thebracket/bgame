#pragma once

#include <rltk.hpp>
#include <string>

using namespace rltk;

struct item_t {
	std::string item_tag;
	item_t() {}
	item_t(const std::string tag) : item_tag(tag) {}

	std::size_t serialization_identity = 9;

	void save(std::ostream &lbfile) {
		serialize(lbfile, item_tag);
	}

	static item_t load(std::istream &lbfile) {
		item_t c;
		deserialize(lbfile, c.item_tag);
		return c;
	}
};
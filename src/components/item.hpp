#pragma once

#include <rltk.hpp>
#include <string>
#include "../raws/raws.hpp"

using namespace rltk;

enum item_type_t {CLOTHING,ITEM};

struct item_t {
	std::string item_name;
	std::string item_tag;
	std::bitset<NUMBER_OF_ITEM_CATEGORIES> category;
	item_type_t type;
	item_t() {}
	item_t(const std::string name) : item_name(name), item_tag(""), type(CLOTHING) {}
	item_t(const std::string tag, const std::string name, const std::bitset<NUMBER_OF_ITEM_CATEGORIES> cats) : 
		item_name(name), item_tag(tag), category(cats), type(ITEM) {}

	std::size_t serialization_identity = 9;

	void save(std::ostream &lbfile) {
		serialize(lbfile, item_name);
		serialize(lbfile, item_tag);
		serialize(lbfile, category);
		serialize(lbfile, type);
	}

	static item_t load(std::istream &lbfile) {
		item_t c;
		deserialize(lbfile, c.item_name);
		deserialize(lbfile, c.item_tag);
		deserialize(lbfile, c.category);
		deserialize(lbfile, c.type);
		return c;
	}
};
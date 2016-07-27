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
	std::size_t material;

	item_t() {}
	item_t(const std::string name) : item_tag(name), type(CLOTHING) {
		//std::cout << "[" << item_tag << "]\n";
		auto finder = item_defs.find(item_tag);
		if (finder != item_defs.end()) {
			item_name = finder->second.name;
		} else {
			auto finder2 = clothing_types.find(item_tag);
			if (finder2 != clothing_types.end()) {
				item_name = finder2->second.name;
			} else {
				item_name = "ERROR";
				std::cout << item_tag << " not found!\n";
			}
		}
	}
	item_t(const std::string tag, const std::string name, const std::bitset<NUMBER_OF_ITEM_CATEGORIES> cats, const std::size_t mat) : 
		item_name(name), item_tag(tag), category(cats), type(ITEM), material(mat) {}

	std::size_t serialization_identity = 9;

	void save(std::ostream &lbfile) {
		serialize(lbfile, item_name);
		serialize(lbfile, item_tag);
		serialize(lbfile, category);
		serialize(lbfile, type);
		serialize(lbfile, material);
	}

	static item_t load(std::istream &lbfile) {
		item_t c;
		deserialize(lbfile, c.item_name);
		deserialize(lbfile, c.item_tag);
		deserialize(lbfile, c.category);
		deserialize(lbfile, c.type);
		deserialize(lbfile, c.material);
		return c;
	}
};
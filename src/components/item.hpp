#pragma once

#include <rltk.hpp>
#include <string>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../raws/items.hpp"

using namespace rltk;

enum item_type_t {CLOTHING,ITEM};

color_t colname_to_col(const std::string &col);

struct item_t {
	std::string item_name;
	std::string item_tag;
	std::bitset<64> category;
	item_type_t type;
	std::size_t material;
	bool claimed = false;
	int stack_size = 1;
	uint16_t clothing_glyph = 0;
    color_t clothing_color;

	item_t() {}
	item_t(const std::string name);
	item_t(const std::string tag, const std::string name, const std::bitset<64> cats,
		const std::size_t mat, int stack=1);

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( item_name, item_tag, category, type, material, claimed, stack_size, clothing_glyph, clothing_color ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<item_t>>)
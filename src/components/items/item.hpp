#pragma once

#include "../../bengine/color_t.hpp"
#include <string>

enum item_type_t {CLOTHING,ITEM};

bengine::color_t colname_to_col(const std::string &col);

struct item_t {
	std::string item_name;
	std::string item_tag;
	item_type_t type;
	std::size_t material;
	int stack_size = 1;
	uint16_t clothing_glyph = 0;
    bengine::color_t clothing_color;
	int clothing_layer = 0;

	item_t() = default;
	explicit item_t(const std::string name) noexcept;
	item_t(const std::string tag, const std::string name, const std::size_t mat, int stack=1, int clothing=0) noexcept;

};

#pragma once

#include <string>
#include "component_types.h"

using std::string;

using namespace serialization_generic;

struct item_component
{
	item_component()
	{
	}
	item_component(const string &t, const char slot = 0, bool tint = false,
			const unsigned char r = 0, const unsigned char g = 0,
			const unsigned char b = 0) :
			item_type(t), clothing_slot(slot), is_tinted(tint), tint_red(r), tint_green(
					g), tint_blue(b)
	{
	}

	int entity_id;
	int handle;
	component_type type = item;
	bool deleted = false;

	bool claimed = false;
	string item_type;
	char clothing_slot;
	bool is_tinted;
	unsigned char tint_red;
	unsigned char tint_green;
	unsigned char tint_blue;

	void save(fstream &lbfile)
	{
		save_common_component_properties<item_component>(lbfile, *this);
		save_primitive<bool>(lbfile, claimed);
		save_primitive<string>(lbfile, item_type);
		save_primitive<char>(lbfile, clothing_slot);
		save_primitive<bool>(lbfile, is_tinted);
		save_primitive<unsigned char>(lbfile, tint_red);
		save_primitive<unsigned char>(lbfile, tint_green);
		save_primitive<unsigned char>(lbfile, tint_blue);
	}

	void load(fstream &lbfile)
	{
		load_common_component_properties<item_component>(lbfile, *this);
		load_primitive<bool>(lbfile, claimed);
		load_primitive<string>(lbfile, item_type);
		load_primitive<char>(lbfile, clothing_slot);
		load_primitive<bool>(lbfile, is_tinted);
		load_primitive<unsigned char>(lbfile, tint_red);
		load_primitive<unsigned char>(lbfile, tint_green);
		load_primitive<unsigned char>(lbfile, tint_blue);
	}
};

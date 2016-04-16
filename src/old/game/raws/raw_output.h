#pragma once

#include <iostream>
#include "base_raw.h"
#include "../components/provisions_component.h"
#include "raw_glyph.h"
#include "raw_color_pair.h"

using std::vector;

namespace raws
{

struct raw_output: public base_raw
{
	raw_output()
	{
		type = OUTPUT;
	}
	;

	raw_output(const string &item, const int &qty) :
			created_item(item), quantity(qty)
	{
		type = OUTPUT;
	}

	string created_item;
	int quantity;
};

}

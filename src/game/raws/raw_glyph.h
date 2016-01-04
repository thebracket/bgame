#pragma once

#include <string>
#include "base_raw.h"

using std::string;

namespace raws
{

struct raw_glpyh: public base_raw
{
	raw_glpyh()
	{
		type = GLYPH;
	}
	;
	raw_glpyh(const unsigned char new_char) :
			character(new_char)
	{
		type = GLYPH;
	}
	unsigned char character;
};

}

#pragma once

#include <string>
#include "base_raw.h"
#include "../components/debug_name_component.h"
#include "../game.h"

using std::string;

namespace raws
{

struct raw_clothing: public base_raw
{
	raw_clothing()
	{
		type = CLOTHING;
	}
	;
	raw_clothing(const string &where, const string &new_name,
			const int &gender_req) :
			slot(where), item(new_name), gender(gender_req)
	{
		type = CLOTHING;
	}
	string slot;
	string item;
	int gender; // 0 = any, 1 = male, 2 = female
};

}

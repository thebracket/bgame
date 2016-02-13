#pragma once

#include <string>

using std::string;

struct chop_order_message
{
	chop_order_message()
	{
	}
	chop_order_message(const int16_t &X, const int16_t &Y, const uint8_t &Z, const int &eid, const string &name) :
			x(X), y(Y), z(Z), tree_id(eid), structure_name(name)
	{
		std::cout << "Chop order for tree #" << tree_id << "\n";
	}

	bool deleted = false;
	int ttl = 2;

	int16_t x;
	int16_t y;
	uint8_t z;
	int tree_id;
	string structure_name;
};

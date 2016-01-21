#pragma once

#include <string>

using std::string;

struct build_order_message
{
	build_order_message()
	{
	}
	build_order_message(const int &X, const int &Y, const int &Z, const string &name) :
			x(X), y(Y), z(Z), structure_name(name)
	{
	}

	bool deleted = false;
	int ttl = 2;

	int16_t x;
	int16_t y;
	uint8_t z;
	string structure_name;
};

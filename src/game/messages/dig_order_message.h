#pragma once

#include <string>

using std::string;

struct dig_order_message
{
	dig_order_message()
	{
	}
	dig_order_message(const int16_t &X, const int16_t &Y, const uint8_t &Z) :
			x(X), y(Y), z(Z)
	{
	}

	bool deleted = false;
	int ttl = 2;

	int16_t x;
	int16_t y;
	uint8_t z;
};

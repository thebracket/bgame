#pragma once

#include <string>

using std::string;

namespace dig_types {
constexpr uint8_t MINE = 0;
constexpr uint8_t CHANNEL = 1;
constexpr uint8_t RAMP = 2;
constexpr uint8_t UPSTAIRS = 3;
constexpr uint8_t DOWNSTAIRS = 4;
constexpr uint8_t UPDOWNSTAIRS = 5;
}

struct dig_order_message
{
	dig_order_message()
	{
	}
	dig_order_message(const int16_t &X, const int16_t &Y, const uint8_t &Z, const uint8_t type=0) :
			x(X), y(Y), z(Z), dig_type(type)
	{
	}

	bool deleted = false;
	int ttl = 2;

	int16_t x;
	int16_t y;
	uint8_t z;
	uint8_t dig_type;
};

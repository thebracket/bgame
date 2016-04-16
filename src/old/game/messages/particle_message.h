#pragma once

#include <string>
#include "chat_emote_message.h"

using std::string;

struct particle_message
{
	particle_message()
	{
	}
	particle_message(const string &msg, const int16_t &tx, const int16_t &ty,
			const uint8_t &tz, const int TTL = 64,
			const chat_emote_color_t col = WHITE) :
			message(msg), tile_x(tx), tile_y(ty), tile_z(tz), offset_x(0), offset_y(
					0), color(col), ttl(TTL)
	{
	}

	string message;
	int16_t tile_x, tile_y;
	uint8_t tile_z;
	int offset_x, offset_y;
	chat_emote_color_t color;

	bool deleted = false;
	int ttl = 64;
};

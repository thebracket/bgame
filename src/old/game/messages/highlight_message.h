#pragma once

#include <string>
#include "chat_emote_message.h"

using std::string;

struct highlight_message
{
	highlight_message()
	{
	}
	highlight_message(const int16_t &tx, const int16_t &ty, const uint8_t &tz,
			const int TTL = 64, const chat_emote_color_t col = WHITE) :
			tile_x(tx), tile_y(ty), tile_z(tz), color(col), ttl(TTL)
	{
	}

	int16_t tile_x, tile_y;
	uint8_t tile_z;
	chat_emote_color_t color;

	bool deleted = false;
	int ttl = 64;
};

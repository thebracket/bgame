#pragma once

struct lighting_changed_message
{
	lighting_changed_message()
	{
	}

	bool deleted = false;
	int ttl = 2;
};

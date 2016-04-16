#pragma once

struct entity_moved_message
{
	entity_moved_message()
	{
	}

	bool deleted = false;
	int ttl = 2;
};

#pragma once

struct walkability_changed_message
{
	walkability_changed_message()
	{
	}

	bool deleted = false;
	int ttl = 2;
};

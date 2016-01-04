#pragma once

#include <string>

using std::string;

struct item_changed_message
{
	item_changed_message()
	{
	}
	item_changed_message(const int &entity) :
			entity_id(entity)
	{
	}

	bool deleted = false;
	int ttl = 2;

	int entity_id;
};

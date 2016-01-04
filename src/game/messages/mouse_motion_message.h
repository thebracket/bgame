#pragma once

struct mouse_motion_message
{
	mouse_motion_message()
	{
	}
	mouse_motion_message(const int &X, const int &Y) :
			x(X), y(Y)
	{
	}

	bool deleted = false;
	int ttl = 1;

	int x;
	int y;
};

#pragma once

enum command_t
{
	NONE,
	TOGGLE_PAUSE,
	CAMERA_UP,
	CAMERA_DOWN,
	CAMERA_LEFT,
	CAMERA_RIGHT,
	CAMERA_Z_UP,
	CAMERA_Z_DOWN,
	LEFT_CLICK,
	RIGHT_CLICK,
	TOGGLE_RENDER_MODE
};

struct command_message
{
	command_message()
	{
		command = NONE;
	}
	command_message(const command_t cmd) :
			command(cmd)
	{
	}

	bool deleted = false;
	int ttl = 1;

	command_t command;
};

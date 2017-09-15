#pragma once

namespace systems {
	extern int mouse_x;
	extern int mouse_y;
	extern bool left_click;
	extern bool right_click;
	extern bool middle_click;
	extern bool wheel_up;
	extern bool wheel_down;

	void poll_mouse();
}
#pragma once

namespace systems {
	extern int mouse_x;
	extern int mouse_y;
	extern bool left_click;
	extern bool right_click;
	extern bool middle_click;
	extern bool wheel_up;
	extern bool wheel_down;
	extern int mouse_wx;
	extern int mouse_wy;
	extern int mouse_wz;
	extern bool mouse_moved;

	void poll_mouse();
}
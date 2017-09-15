#pragma once

namespace systems {
	void register_keypress();
	bool ready_for_keyboard_input();
	void add_time(double ms);
	bool ready_for_keyboard_input();
	bool is_key_down(int key, bool damp=true);
}

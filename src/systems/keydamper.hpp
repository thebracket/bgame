#pragma once

namespace systems {
	void register_keypress() noexcept;
	bool ready_for_keyboard_input() noexcept;
	void add_time(double ms) noexcept;
	bool ready_for_keyboard_input() noexcept;
	bool is_key_down(const int key, const bool damp=true) noexcept;
}

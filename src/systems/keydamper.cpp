#include "stdafx.h"
#include "keydamper.hpp"
#include "../bengine/imgui.h"

namespace systems {
	constexpr double KEY_DELAY = 75.0;
	double ms_since_press = 1000.0;

	void register_keypress() noexcept {
		ms_since_press = 0.0;
	}

	void add_time(double ms) noexcept {
		ms_since_press += ms;
	}

	bool ready_for_keyboard_input() noexcept {
		const auto result = (ms_since_press > KEY_DELAY);
		return result;
	}

	bool is_key_down(const int key, const bool damp) noexcept {
		if (damp && !ready_for_keyboard_input() || ImGui::GetIO().WantCaptureKeyboard) return false;
		if (ImGui::IsKeyDown(key)) {
			register_keypress();
			return true;
		}
		return false;
	}
}
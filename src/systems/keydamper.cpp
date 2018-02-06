#include "keydamper.hpp"
#include "../bengine/imgui.h"
#include "../bengine/imgui_impl_glfw_gl3.h"

namespace systems {
	constexpr double KEY_DELAY = 75.0;
	double ms_since_press = 1000.0;

	void register_keypress() {
		ms_since_press = 0.0;
	}

	void add_time(double ms) {
		ms_since_press += ms;
	}

	bool ready_for_keyboard_input() {
		return (ms_since_press > KEY_DELAY);
	}

	bool is_key_down(int key, bool damp) {
		if (damp && !ready_for_keyboard_input()) return false;
		if (ImGui::IsKeyDown(key)) {
			register_keypress();
			return true;
		}
		return false;
	}
}
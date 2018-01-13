#include "stdafx.h"
#include "mouse.hpp"
#include "../bengine/imgui.h"
#include "../bengine/imgui_impl_glfw_gl3.h"
#include "../bengine/main_window.hpp"
#include "../render_engine/render_engine.hpp"
#include "../global_assets/game_camera.hpp"

namespace systems {
	int mouse_x = 0;
	int mouse_y = 0;
	bool left_click = false;
	bool right_click = false;
	bool middle_click = false;
	bool wheel_up = false;
	bool wheel_down = false;

	int mouse_wx = 0;
	int mouse_wy = 0;
	int mouse_wz = 0;

	void read_gl_position(const int &screen_height) {
		if (camera->ascii_mode) {
			// TODO: Read-back ASCII mode position
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, render::gbuffer->fbo_id);
			float pixels[3];
			glReadPixels(mouse_x, screen_height - mouse_y, 1, 1, GL_RGB, GL_FLOAT, &pixels[0]);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			mouse_wx = static_cast<int>(pixels[0] + 0.5f);
			mouse_wz = static_cast<int>(pixels[1] + 0.6f);
			mouse_wy = static_cast<int>(pixels[2] + 0.5f);

			//std::cout << mouse_wx << " / " << mouse_wy << " / " << mouse_wz << "\n";
		}
	}

	void poll_mouse() {
		int screen_w, screen_h;
		glfwGetWindowSize(bengine::main_window, &screen_w, &screen_h);

		ImGuiIO& io = ImGui::GetIO();
		if (!io.WantCaptureMouse) {
			mouse_x = static_cast<int>(io.MousePos.x);
			mouse_y = static_cast<int>(io.MousePos.y);
			left_click = io.MouseDown[0];
			right_click = io.MouseDown[1];
			middle_click = io.MouseDown[2];
			auto wheel = io.MouseWheel;
			wheel_up = false;
			wheel_down = false;
			if (wheel < 0) wheel_up = true;
			if (wheel > 0) wheel_down = true;
			read_gl_position(screen_h);
		}
		else {
			left_click = false;
			right_click = false;
			middle_click = false;
		}
	}
}
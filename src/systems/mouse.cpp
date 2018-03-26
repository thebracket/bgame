#include "stdafx.h"
#include "mouse.hpp"
#include "../bengine/imgui.h"
#include "../bengine/main_window.hpp"
#include "../render_engine/renderbuffers.hpp"
#include "../global_assets/game_camera.hpp"
#include "../ascii_engine/ascii_mode.hpp"
#include "../planet/region/region.hpp"
#include "keydamper.hpp"
#include "../global_assets/game_mode.hpp"
#include "../global_assets/game_pause.hpp"

namespace systems {
	int mouse_x = 0;
	int mouse_y = 0;
	static int last_mouse_x = 0;
	static int last_mouse_y = 0;
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
			glBindFramebuffer(GL_FRAMEBUFFER, render::ascii::ascii_fbo);
			float pixels[3];
			glReadPixels(mouse_x, screen_height - mouse_y, 1, 1, GL_RGB, GL_FLOAT, &pixels[0]);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			mouse_wx = std::floor(pixels[0] + 0.5f);
			mouse_wz = std::floor(pixels[2] + 0.6f);
			mouse_wy = std::floor(pixels[1] + 0.5f);

			mouse_wz = camera_position->region_z;
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, render::gbuffer->fbo_id);
			float pixels[3];
			glReadPixels(mouse_x, screen_height - mouse_y, 1, 1, GL_RGB, GL_FLOAT, &pixels[0]);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			mouse_wx = std::floor(pixels[0] + 0.5f);
			mouse_wz = std::floor(pixels[1] + 0.6f);
			mouse_wy = std::floor(pixels[2] + 0.5f);			
		}
	}

	static void clip_mouse()
	{
		if (mouse_wx < 1) mouse_wx = 1;
		if (mouse_wx > REGION_WIDTH - 1) mouse_wx = REGION_WIDTH - 1;
		if (mouse_wy < 1) mouse_wy = 1;
		if (mouse_wy > REGION_HEIGHT - 1) mouse_wy = REGION_HEIGHT - 1;
		if (mouse_wz < 1) mouse_wz = 1;
		if (mouse_wz > REGION_DEPTH - 1) mouse_wz = REGION_DEPTH - 1;
	}

	void poll_mouse() {
		int screen_w, screen_h;
		glfwGetWindowSize(bengine::main_window, &screen_w, &screen_h);

		auto& io = ImGui::GetIO();
		if (!io.WantCaptureMouse) {
			mouse_x = static_cast<int>(io.MousePos.x);
			mouse_y = static_cast<int>(io.MousePos.y);
			left_click = io.MouseDown[0];
			right_click = io.MouseDown[1];
			middle_click = io.MouseDown[2];
			const auto wheel = io.MouseWheel;
			wheel_up = false;
			wheel_down = false;
			if (wheel < 0) wheel_up = true;
			if (wheel > 0) wheel_down = true;
			if (last_mouse_x != mouse_x || last_mouse_y != mouse_y || last_mouse_x == 0) {
				read_gl_position(screen_h);
				last_mouse_x = mouse_x;
				last_mouse_y = mouse_y;
			}
			else if (game_master_mode == DESIGN || game_master_mode == LOOK_MODE) {
				pause_mode = PAUSED; // Always pause while designing
				mouse_wz = camera_position->region_z;

				if (is_key_down(GLFW_KEY_ESCAPE)) game_master_mode = PLAY;

				const auto shift = (glfwGetKey(bengine::main_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(bengine::main_window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);
				const auto control = (glfwGetKey(bengine::main_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(bengine::main_window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS);

				if (!shift && !control) {
					if (is_key_down(GLFW_KEY_LEFT)) mouse_wx--;
					if (is_key_down(GLFW_KEY_RIGHT)) mouse_wx++;
					if (is_key_down(GLFW_KEY_UP)) mouse_wy--;
					if (is_key_down(GLFW_KEY_DOWN)) mouse_wy++;
					if (is_key_down(GLFW_KEY_ENTER)) left_click = true;
					if (is_key_down(GLFW_KEY_BACKSPACE)) right_click = true;
				}
			}
			clip_mouse();
		}
		else {
			left_click = false;
			right_click = false;
			middle_click = false;
		}
	}
}

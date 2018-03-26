#include "stdafx.h"
#include "camera_system.hpp"
#include "../../bengine/imgui.h"
#include "../../global_assets/game_camera.hpp"
#include "../../planet/constants.hpp"
#include "../../render_engine/render_engine.hpp"
#include "../keydamper.hpp"
#include "../mouse.hpp"
#include "../../render_engine/design_render.hpp"
#include "../../bengine/analytics.hpp"
#include "../../bengine/gl_include.hpp"
#include "../../bengine/main_window.hpp"
#include "../../bengine/imgui.h"
#include "../../bengine/IconsFontAwesome.h"
#include "../../global_assets/game_mode.hpp"

namespace systems {
    namespace camerasys {
		static void normal_keys()
		{
			if (is_key_down(GLFW_KEY_LEFT, false)) {
				--camera_position->region_x;
				if (camera_position->region_x < 0) camera_position->region_x = 0;
				render::camera_moved = true;
				render::models_changed = true;
			}
			if (is_key_down(GLFW_KEY_RIGHT, false)) {
				++camera_position->region_x;
				if (camera_position->region_x > REGION_WIDTH - 1) camera_position->region_x = REGION_WIDTH - 1;
				render::camera_moved = true;
				render::models_changed = true;
			}

			if (is_key_down(GLFW_KEY_UP, false)) {
				--camera_position->region_y;
				if (camera_position->region_y < 0) camera_position->region_y = 0;
				render::camera_moved = true;
				render::models_changed = true;
			}

			if (is_key_down(GLFW_KEY_DOWN, false)) {
				++camera_position->region_y;
				if (camera_position->region_y > REGION_HEIGHT - 1) camera_position->region_y = REGION_HEIGHT - 1;
				render::camera_moved = true;
				render::models_changed = true;
			}

			if (is_key_down(GLFW_KEY_COMMA)) {
				++camera_position->region_z;
				if (camera_position->region_z > REGION_DEPTH - 1) camera_position->region_z = REGION_DEPTH - 1;
				render::camera_moved = true;
				render::models_changed = true;
				render::mode_change = true;
			}

			if (is_key_down(GLFW_KEY_PERIOD)) {
				--camera_position->region_z;
				if (camera_position->region_z < 1) camera_position->region_z = 1;
				render::camera_moved = true;
				render::models_changed = true;
				render::mode_change = true;
			}

			const auto pressed_control = glfwGetKey(bengine::main_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(bengine::main_window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;
			const auto pressed_shift = glfwGetKey(bengine::main_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(bengine::main_window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
			const auto pressed_tab = glfwGetKey(bengine::main_window, GLFW_KEY_TAB) == GLFW_PRESS;

			if (ready_for_keyboard_input() && pressed_tab && !pressed_control && !pressed_shift) {
				register_keypress();
				switch (camera->camera_mode) {
				case game_camera_mode_t::DIAGONAL_LOOK_NW: {
					camera->camera_mode = game_camera_mode_t::DIAGONAL_LOOK_NE;
					bengine::analytics::on_event("game", "camera_mode", "Diagonal NE");
				}break;
				case game_camera_mode_t::DIAGONAL_LOOK_NE: {
					camera->camera_mode = game_camera_mode_t::DIAGONAL_LOOK_SW;
					bengine::analytics::on_event("game", "camera_mode", "Diagonal SW");
				}break;
				case game_camera_mode_t::DIAGONAL_LOOK_SW: {
					camera->camera_mode = game_camera_mode_t::DIAGONAL_LOOK_SE;
					bengine::analytics::on_event("game", "camera_mode", "Diagonal SE");
				}break;
				case game_camera_mode_t::DIAGONAL_LOOK_SE: {
					camera->camera_mode = game_camera_mode_t::FRONT;
					bengine::analytics::on_event("game", "camera_mode", "Front");
				}break;
				case game_camera_mode_t::FRONT: {
					camera->camera_mode = game_camera_mode_t::TOP_DOWN;
					bengine::analytics::on_event("game", "camera_mode", "Top-Down");
				} break;
				case game_camera_mode_t::TOP_DOWN: {
					camera->camera_mode = game_camera_mode_t::DIAGONAL_LOOK_NW;
					bengine::analytics::on_event("game", "camera_mode", "Diagonal NW");
				} break;
				default: {} break; // We're ignoring the FPS modes
				}
				render::camera_moved = true;
				render::models_changed = true;
			}

			if (ready_for_keyboard_input() && pressed_tab && pressed_shift && !pressed_control) {
				register_keypress();
				camera->ascii_mode = !camera->ascii_mode;
				render::camera_moved = true;
				render::models_changed = true;
				bengine::analytics::on_event("game", "renderMode", camera->ascii_mode ? "ASCII" : "3D");
			}
			if (ready_for_keyboard_input() && pressed_tab && !pressed_shift && pressed_control) {
				register_keypress();
				camera->perspective = !camera->perspective;
				render::camera_moved = true;
				render::models_changed = true;
			}

			if (is_key_down(GLFW_KEY_PAGE_UP, false) || wheel_up) {
				--camera->zoom_level;
				if (camera->zoom_level < 2) camera->zoom_level = 2;
				render::camera_moved = true;
				render::models_changed = true;
			}

			if (is_key_down(GLFW_KEY_PAGE_DOWN, false) || wheel_down) {
				++camera->zoom_level;
				if (camera->zoom_level > 50) camera->zoom_level = 50;
				render::camera_moved = true;
				render::models_changed = true;
			}
		}

		static void follow_mode()
		{
			if (camera->following == 0)
			{
				camera->following = 0;
			} else
			{
				const auto following = bengine::entity(camera->following);
				if (!following)
				{
					camera->following = 0;
				} else
				{
					const auto pos = following->component<position_t>();
					if (!pos)
					{
						camera->following = 0;
					} else
					{
						camera_position->region_x = pos->x;
						camera_position->region_y = pos->y;
						camera_position->region_z = pos->z;
						render::camera_moved = true;
						render::models_changed = true;

						std::string follow_title = std::string(ICON_FA_CAMERA);
						const auto name = following->component<name_t>();
						if (name)
						{
							follow_title += std::string(" Following ") + name->first_name + " " + name->last_name;
						} else
						{
							follow_title += " Following";
						}

						const std::string stop_btn = std::string(ICON_FA_TIMES) + " Stop Following";
						ImGui::Begin(follow_title.c_str());
						if (ImGui::Button("Toggle First Person View"))
						{
							camera->fps = !camera->fps;
						}
						ImGui::SameLine();
						if (ImGui::Button(stop_btn.c_str()))
						{
							camera->following = 0;
							render::camera_moved = true;
							render::models_changed = true;
						}
						ImGui::End();
					}
				}
			}
		}

		void run(const double &duration_ms) {
			if (camera->following > 0)
			{
				follow_mode();
			}
			const auto shift = (glfwGetKey(bengine::main_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(bengine::main_window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);
			const auto control = (glfwGetKey(bengine::main_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(bengine::main_window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS);

			if ((game_master_mode != DESIGN && game_master_mode != LOOK_MODE) || (shift && !control)) {
				normal_keys();
			}
		}
    }
}

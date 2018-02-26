#include "design_mode.hpp"
#include "../keydamper.hpp"
#include "../../global_assets/game_mode.hpp"
#include "../../global_assets/game_pause.hpp"
#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../../bengine/imgui_impl_glfw_gl3.h"
#include "../../render_engine/design_render.hpp"
#include "../../bengine/gl_include.hpp"
#include <string>
#include "../../bengine/main_window.hpp"
#include "../mouse.hpp"

namespace systems {
	namespace design_mode {

		const std::string win_design = std::string(ICON_FA_PENCIL_SQUARE) + " Design Mode";
		const std::string btn_close = std::string(ICON_FA_TIMES) + " Close";

		void run(const double &duration_ms) {
			if (game_master_mode == PLAY || game_master_mode == DESIGN)
			{
				if (glfwGetKey(bengine::main_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(bengine::main_window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
					if (is_key_down(GLFW_KEY_T, false)) game_design_mode = CHOPPING;
					if (is_key_down(GLFW_KEY_B, false)) game_design_mode = BUILDING;
					if (is_key_down(GLFW_KEY_G, false)) game_design_mode = GUARDPOINTS;
					if (is_key_down(GLFW_KEY_H, false)) game_design_mode = HARVEST;
					if (is_key_down(GLFW_KEY_M, false)) game_design_mode = DIGGING;
					if (is_key_down(GLFW_KEY_S, false)) game_design_mode = STOCKPILES;
					if (is_key_down(GLFW_KEY_A, false)) game_design_mode = ARCHITECTURE;
				}
			}

			if (game_master_mode == PLAY) {
				if (is_key_down(GLFW_KEY_D, false)) {
					game_master_mode = DESIGN;
					render::mode_change = true;
				}
			}
			else if (game_master_mode == DESIGN) {
				pause_mode = PAUSED; // Always pause while designing
				
				if (is_key_down(GLFW_KEY_ESCAPE)) game_master_mode = PLAY;

				const auto shift = (glfwGetKey(bengine::main_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(bengine::main_window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);

				if (!shift) {
					if (is_key_down(GLFW_KEY_LEFT)) mouse_wx--;
					if (is_key_down(GLFW_KEY_RIGHT)) mouse_wx++;
					if (is_key_down(GLFW_KEY_UP)) mouse_wy--;
					if (is_key_down(GLFW_KEY_DOWN)) mouse_wy++;
					if (is_key_down(GLFW_KEY_ENTER)) left_click = true;
					if (is_key_down(GLFW_KEY_BACKSPACE)) right_click = true;
				}
			}
		}
	}
}

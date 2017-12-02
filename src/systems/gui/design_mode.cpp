#include "design_mode.hpp"
#include "../keydamper.hpp"
#include "../../bengine/gl_include.hpp"
#include "../../global_assets/game_mode.hpp"
#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../../bengine/imgui_impl_glfw_gl3.h"

namespace systems {
	namespace design_mode {

		const std::string win_design = std::string(ICON_FA_PENCIL_SQUARE) + " Design Mode";
		const std::string btn_close = std::string(ICON_FA_TIMES) + " Close";

		void run(const double &duration_ms) {
			if (game_master_mode == PLAY) {
				if (is_key_down(GLFW_KEY_D, false)) game_master_mode = DESIGN;
			}
			else if (game_master_mode == DESIGN) {
				if (is_key_down(GLFW_KEY_T, false)) game_design_mode = CHOPPING;
				if (is_key_down(GLFW_KEY_B, false)) game_design_mode = BUILDING;
				if (is_key_down(GLFW_KEY_G, false)) game_design_mode = GUARDPOINTS;
				if (is_key_down(GLFW_KEY_H, false)) game_design_mode = HARVEST;
				if (is_key_down(GLFW_KEY_M, false)) game_design_mode = DIGGING;
				if (is_key_down(GLFW_KEY_S, false)) game_design_mode = STOCKPILES;
				if (is_key_down(GLFW_KEY_A, false)) game_design_mode = ARCHITECTURE;
				if (is_key_down(GLFW_KEY_ESCAPE)) game_master_mode = PLAY;

				ImGui::Begin(win_design.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
				ImGui::RadioButton("(M)ining", (int *)&game_design_mode, 0); ImGui::SameLine();
				ImGui::RadioButton("(B)uilding", (int *)&game_design_mode, 1); ImGui::SameLine();
				ImGui::RadioButton("(T)ree Chopping", (int *)&game_design_mode, 2); ImGui::SameLine();
				ImGui::RadioButton("(G)uardpoints", (int *)&game_design_mode, 3); ImGui::SameLine();
				ImGui::RadioButton("(S)tockpiles", (int *)&game_design_mode, 4); ImGui::SameLine();
				ImGui::RadioButton("(H)arvest", (int *)&game_design_mode, 5); ImGui::SameLine();
				ImGui::RadioButton("(A)rchitecture", (int *)&game_design_mode, 6);

				if (ImGui::Button(btn_close.c_str())) {
					game_master_mode = PLAY;
				}
				ImGui::End();
			}
		}
	}
}
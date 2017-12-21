#include "stdafx.h"
#include "../../global_assets/game_designations.hpp"
#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../../bengine/imgui_impl_glfw_gl3.h"
#include "../mouse.hpp"
#include "../keydamper.hpp"
#include "../../global_assets/game_mode.hpp"
#include "../ai/mining_system.hpp"

namespace systems {
	namespace design_mining {
		static const std::string win_mining = std::string(ICON_FA_DIAMOND) + " Mining";

		static bool is_mining_designation_valid(const int &x, const int &y, const int &z, const game_mining_mode_t &mode) {
			const int idx = mapidx(x, y, z);
			const uint8_t tt = region::tile_type(idx);
			if (mode == DIG || mode == UP || mode == UPDOWN) {
				if (tt == tile_type::SOLID || tt == tile_type::WALL) return true;
				return false;
			}
			else if (mode == CHANNEL || mode == RAMP || mode == DOWN) {
				if (tt == tile_type::SOLID || tt == tile_type::WALL || tt == tile_type::FLOOR) return true;
				return false;
			}
			return true;
		}

		void run(const double &duration_ms) {
			// Keyboard handler
			if (is_key_down(GLFW_KEY_D)) game_mining_mode = DIG;
			if (is_key_down(GLFW_KEY_C)) game_mining_mode = CHANNEL;
			if (is_key_down(GLFW_KEY_R)) game_mining_mode = RAMP;
			if (is_key_down(GLFW_KEY_U)) game_mining_mode = UP;
			if (is_key_down(GLFW_KEY_J)) game_mining_mode = DOWN;
			if (is_key_down(GLFW_KEY_I)) game_mining_mode = UPDOWN;
			if (is_key_down(GLFW_KEY_X)) game_mining_mode = MINING_DELETE;

			if (left_click) {
				const auto idx = mapidx(mouse_wx, mouse_wy, mouse_wz);
				if (is_mining_designation_valid(mouse_wx, mouse_wy, mouse_wz, game_mining_mode)) {
					switch (game_mining_mode) {
					case DIG: designations->mining[idx] = 1; break;
					case CHANNEL: designations->mining[idx] = 2; break;
					case RAMP: designations->mining[idx] = 3; break;
					case UP: designations->mining[idx] = 4; break;
					case DOWN: designations->mining[idx] = 5; break;
					case UPDOWN: designations->mining[idx] = 6; break;
					case MINING_DELETE: designations->mining.erase(idx); break;
					}
					//std::cout << "Added mining designation\n";
					mining_system::mining_map_changed();
				}
			}

			ImGui::Begin(win_mining.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
			switch (game_mining_mode) {
			case DIG: ImGui::Text("Currently: DIGGING"); break;
			case CHANNEL: ImGui::Text("Currently: CHANNELING"); break;
			case RAMP: ImGui::Text("Currently: RAMPING"); break;
			case UP: ImGui::Text("Currently: Carving UP stairs"); break;
			case DOWN: ImGui::Text("Currently: Carving DOWN stairs"); break;
			case UPDOWN: ImGui::Text("Currently: Carving UP/DOWN stairs"); break;
			case MINING_DELETE: ImGui::Text("Currently: Removing designations"); break;
			}
			if (ImGui::Button("Dig (d)")) game_mining_mode = DIG;
			if (ImGui::Button("Channel (c)")) game_mining_mode = CHANNEL;
			if (ImGui::Button("Ramp (r)")) game_mining_mode = RAMP;
			if (ImGui::Button("Up Stair (u)")) game_mining_mode = UP;
			if (ImGui::Button("Down Stair (j)")) game_mining_mode = DOWN;
			if (ImGui::Button("Up/Down Stair (i)")) game_mining_mode = UPDOWN;
			if (ImGui::Button("Clear Designation (x)")) game_mining_mode = MINING_DELETE;
			ImGui::End();
		}
	}
}
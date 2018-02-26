#include "log_system.hpp"
#include "../../global_assets/game_logger.hpp"
#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../../bengine/imgui_impl_glfw_gl3.h"
#include "../../global_assets/game_mode.hpp"
#include "../../bengine/rexspeeder.hpp"
#include "../../bengine/main_window.hpp"
#include "../../bengine/color_t.hpp"
#include <mutex>
#include <queue>

namespace systems {
	namespace logging {
		std::queue<log_message> log_entries;
		std::mutex log_mutex;
		bool first_run = true;
		const std::string win_log = std::string(ICON_FA_COMMENT) + " Log";

		void log(log_message &message) {
			std::lock_guard<std::mutex> lock(log_mutex);
			log_entries.push(message);
		}

		void age_log() {
			for (auto &line : logger->lines) {
				++line.age;
			}
		}

		void process_log_messages() {
			std::lock_guard<std::mutex> lock(log_mutex);
			while (!log_entries.empty()) {
				log_message msg = log_entries.front();
				log_entries.pop();

				logger->lines.insert(logger->lines.begin(), log_line_t{ msg.chars });
			}
		}

		void run(const double &duration_ms) {
			process_log_messages();

			if (game_master_mode != PLAY) return;

			//std::cout << "Log system render\n";
			if (logger->lines.empty()) return;
			if (first_run) {
				int w, h;
				glfwGetWindowSize(bengine::main_window, &w, &h);
				ImGui::SetNextWindowPos(ImVec2(5, h - 150), ImGuiSetCond_Always);
				first_run = false;
			}
			ImGui::Begin(win_log.c_str(), nullptr, ImVec2{ 600,125 }, 0.5f, ImGuiWindowFlags_NoCollapse + ImGuiWindowFlags_NoSavedSettings);
			for (const auto &line : logger->lines) {
				if (!line.chars.empty()) {
					std::string output;
					bengine::color_t current = line.chars[0].foreground;
					bool first = true;
					for (const xp::vchar &c : line.chars) {
						if (c.foreground == current) {
							output += c.glyph;
						}
						else {
							if (!first) {
								ImGui::SameLine();
							}
							else {
								first = false;
							}
							ImVec4 col{ current.r, current.g, current.b, 1.0f };
							ImGui::TextColored(col, "%s", output.c_str());
							output = c.glyph;
							current = c.foreground;
						}
					}
					if (!output.empty()) {
						if (!first) {
							ImGui::SameLine();
						}
						else {
							first = false;
						}
						ImVec4 col{ current.r, current.g, current.b, 1.0f };
						ImGui::TextColored(col, "%s", output.c_str());
						output = "";
					}
				}
			}
			ImGui::End();
		}
	}
}

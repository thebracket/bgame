#include "stdafx.h"
#include "first_run_screen.hpp"
#include "../bengine/filesystem.hpp"
#include "splash_screen.hpp"
#include "../bengine/main_window.hpp"
#include "../bengine/imgui.h"
#include "../bengine/imgui_impl_glfw_gl3.h"
#include "../global_assets/game_config.hpp"
#include <fstream>
#include <string>

namespace first_run_screen {
	bool first_run_done = false;

	bool is_first_run() noexcept {
		const auto path = get_save_path() + "/firstrun.txt";
		if (exists(path)) return false;

		std::ofstream f(path);
		f << "We've warned about Internet usage\n";

		return true;
	}

	void tick(const double &duration_ms) noexcept {
		using namespace config;

		ImGui_ImplGlfwGL3_NewFrame();

		ImGui::Begin("Starting Nox Futura for the first time...");
		ImGui::TextColored(ImVec4(1.0, 1.0, 0.0, 1.0), "Before we get started, may we use your Internet connection?");
		ImGui::Text("We use Google Analytics to give us an idea of how many people are playing the game, and to track bugs. It also gives us a warm fuzzy feeling to know that people are actually playing the game.");
		ImGui::Text("We also understand that you might pay for data usage, or might object to being tracked. That's cool - we worry about that, too.");

		ImGui::Text("Allow telemetry to phone home");
		ImGui::SameLine();
		ImGui::Checkbox("## Allow telemetry to phone home", &game_config.allow_calling_home);

		if (ImGui::Button("Save Preferences and Run the Game")) {
			game_config.save();
			first_run_done = true;
		}

		ImGui::End();
		ImGui::Render();

		// Are we done? If so, move on...
		if (first_run_done) {
			splash_screen::init();
			bengine::main_func = splash_screen::tick;
		}
	}
}

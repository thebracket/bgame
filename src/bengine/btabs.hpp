#pragma once
#include <string>
#include "gl_include.hpp"
#include "imgui.h"
#include "main_window.hpp"
#include "../global_assets/game_pause.hpp"
#include <vector>
#include <functional>
#include <optional>
#include "../systems/keydamper.hpp"

/*
 * Simple tabs system for ImGui, until a real one exists.
 */

namespace bengine
{
	/*
	 * Helper function to handle the creation of large info windows, to keep them consisent.
	 * show_window is a locally stored bool (must be true!) that will become false if the close button was clicked. You should then reset it to true.
	 */
	inline void begin_info_window(const std::string &title, bool * show_window, const bool should_pause=true) noexcept
	{
		int screen_w, screen_h;
		glfwGetWindowSize(bengine::main_window, &screen_w, &screen_h);

		ImGui::SetNextWindowPos(ImVec2{ 50.0f, 50.0f });
		ImGui::Begin(title.c_str(), show_window, ImVec2{ screen_w - 100.0f, screen_h - 100.0f }, 0.75f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal);
		if (should_pause) pause_mode = PAUSED; // Always pause in an info-window
		if (glfwGetKey(bengine::main_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			*show_window = false; // Escape always closes the window
		}
	}

	struct table_heading_t
	{
		const char * title;
		float width;
	};

	/*
	 * Helper function to help me get consistent tables!
	 */
	inline void begin_table(bool &first_run, const std::vector<table_heading_t> &headings, const std::string &id, const bool border = true) noexcept
	{
		const auto header_id = id + std::string("_h");

		const auto header_color = ImColor(0.1f, 0.1f, 0.5f, 1.0f);
		const auto pos = ImGui::GetCursorScreenPos();
		const auto w = ImGui::GetWindowWidth();
		const auto sz = ImVec2{ w, ImGui::GetTextLineHeight() };
		ImGui::GetWindowDrawList()->AddRectFilled(pos, ImVec2{ pos.x + sz.x, pos.y + sz.y }, header_color);

		ImGui::Columns(headings.size(), header_id.c_str(), border);
		auto i = 0;
		auto offset = 0.0f;
		for (const auto &th : headings)
		{
			if (first_run) {
				ImGui::SetColumnOffset(i, offset);
				if (th.width > -1.0f)
				{
					offset += th.width;
				}
				else
				{
					const auto heading_size = ImGui::CalcTextSize(th.title, nullptr, true);
					offset += heading_size.x + 20.0f;
				}
				first_run = false;
			}

			ImGui::TextColored(ImVec4{ 0.0f, 1.0f, 1.0f, 1.0f }, "%s", th.title);
			ImGui::NextColumn();
			++i;
		}
		ImGui::Separator();
	}

	/* Starts a row */
	inline void zebra_row(bool &row_toggler)
	{
		row_toggler = !row_toggler;
	}

	/* Starts a column */
	inline void begin_zebra_col(bool &row_toggler)
	{
		const auto a_color = ImColor(0.2f, 0.2f, 0.3f, 1.0f);
		const auto b_color = ImColor(0.15f, 0.15f, 0.25f, 1.0f);
		const auto pos = ImGui::GetCursorScreenPos();
		const auto w = ImGui::GetWindowWidth();
		const auto sz = ImVec2{ w, ImGui::GetTextLineHeightWithSpacing() };

		if (row_toggler) {
			ImGui::GetWindowDrawList()->AddRectFilled(pos, ImVec2{ pos.x + sz.x, pos.y + sz.y }, a_color);
		}
		else
		{
			ImGui::GetWindowDrawList()->AddRectFilled(pos, ImVec2{ pos.x + sz.x, pos.y + sz.y }, b_color);
		}
	}

	/* Ends a column */
	inline void end_zebra_col()
	{
		ImGui::NextColumn();
	}

	/* Ends a table */
	inline void end_table()
	{
		ImGui::Columns(1);
	}

	/* Defines a tab */
	struct btab_t
	{
		std::string title;
		std::function<void()> on_render;
		std::optional<std::pair<std::string, int>> shortcut;
	};

	/* Defines a tab set */
	struct btabs_t
	{
		std::vector<btab_t> tabs;
		int selected = 0;
	};

	/* Render a tab bar, send callbacks to active tab */
	inline void render_btab_bar(btabs_t &bt) noexcept
	{
		auto i = 0;
		const auto sz = bt.tabs.size()-1;
		for (auto &b : bt.tabs)
		{
			const auto full_title = b.shortcut ? std::string("(") + b.shortcut->first + std::string(") ") + b.title : b.title;
			if (i == bt.selected)
			{
				ImGui::TextColored(ImVec4{ 1.0f, 1.0f, 0.0f, 1.0f }, "%s", b.title.c_str());
			} else if (ImGui::Button(full_title.c_str()))
			{
				bt.selected = i;
			}
			if (b.shortcut && systems::is_key_down(b.shortcut->second)) bt.selected = i;
			if (i < sz) ImGui::SameLine();
			++i;
		}
		ImGui::Separator();
		bt.tabs[bt.selected].on_render();
	}

	/* Used for design mode width controls, ensures consistent keyboard support. */
	inline void render_width_control(int &width, const int min, const int max)
	{
		ImGui::Text("Width: %d (O/P)", width);
		ImGui::SameLine();
		if (ImGui::SmallButton("+##WP")) --width;
		ImGui::SameLine();
		if (ImGui::SmallButton("-##WM")) ++width;
		if (systems::is_key_down(GLFW_KEY_O)) --width;
		if (systems::is_key_down(GLFW_KEY_P)) ++width;
		if (width < min) width = min;
		if (width > max) width = max;
	}

	/* Used for design mode height controls, ensures consistent keyboard support. */
	inline void render_height_control(int &height, const int min, const int max)
	{
		ImGui::Text("Height: %d ([/])", height);
		ImGui::SameLine();
		if (ImGui::SmallButton("+##HP")) ++height;
		ImGui::SameLine();
		if (ImGui::SmallButton("-##HM")) --height;
		if (systems::is_key_down(GLFW_KEY_LEFT_BRACKET)) --height;
		if (systems::is_key_down(GLFW_KEY_RIGHT_BRACKET)) ++height;
		if (height < min) height = min;
		if (height > max) height = max;
	}

	/* Used for design mode that needs a radius */
	inline void render_radius_control(int &radius, const int min, const int max)
	{
		ImGui::Text("Radius: %d ([/])", radius);
		ImGui::SameLine();
		if (ImGui::SmallButton("+##RP")) ++radius;
		ImGui::SameLine();
		if (ImGui::SmallButton("-##RM")) --radius;
		if (systems::is_key_down(GLFW_KEY_LEFT_BRACKET)) --radius;
		if (systems::is_key_down(GLFW_KEY_RIGHT_BRACKET)) ++radius;
		if (radius < min) radius = min;
		if (radius > max) radius = max;
	}

}
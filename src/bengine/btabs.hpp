#pragma once
#include <string>
#include "gl_include.hpp"
#include "imgui.h"
#include "main_window.hpp"
#include <vector>
#include <functional>

/*
 * Simple tabs system for ImGui, until a real one exists.
 */

namespace bengine
{
	/*
	 * Helper function to handle the creation of large info windows, to keep them consisent.
	 * show_window is a locally stored bool (must be true!) that will become false if the close button was clicked. You should then reset it to true.
	 */
	inline void begin_info_window(const std::string &title, bool * show_window)
	{
		int screen_w, screen_h;
		glfwGetWindowSize(bengine::main_window, &screen_w, &screen_h);

		ImGui::SetNextWindowPos(ImVec2{ 50.0f, 50.0f });
		ImGui::Begin(title.c_str(), show_window, ImVec2{ screen_w - 100.0f, screen_h - 100.0f }, 0.75f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal);
	}

	struct btab_t
	{
		std::string title;
		std::function<void()> on_render;
	};

	struct btabs_t
	{
		std::vector<btab_t> tabs;
		int selected = 0;
	};

	inline void render_btab_bar(btabs_t &bt) noexcept
	{
		std::size_t i = 0;
		const auto sz = bt.tabs.size()-1;
		for (auto &b : bt.tabs)
		{
			if (i == bt.selected)
			{
				ImGui::TextColored(ImVec4{ 1.0f, 1.0f, 0.0f, 1.0f }, "%s", b.title.c_str());
			} else if (ImGui::Button(b.title.c_str()))
			{
				bt.selected = i;
			}
			if (i < sz) ImGui::SameLine();
			++i;
		}
		ImGui::Separator();
		bt.tabs[bt.selected].on_render();
	}

}
#pragma once
#include <string>
#include "imgui.h"
#include <vector>
#include <functional>

/*
 * Simple tabs system for ImGui, until a real one exists.
 */

namespace bengine
{

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
			if (ImGui::Button(b.title.c_str()))
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
#pragma once

#include <vector>
#include <utility>
#include <sstream>
#include <memory>
#include "../world/world.h"
#include "../../engine/sdl2_backend.h"
#include "../game.h"
#include "../raws/raws.h"
#include "terrain_info_window.h"
#include "structure_info_window.h"
#include "settler_info_window.h"
#include "build_options_window.h"

using std::vector;
using std::stringstream;

namespace render
{

class panel_tile_info
{
public:
	panel_tile_info(sdl2_backend * sdl, const std::pair<int, int> region_loc,
			const std::pair<int, int> mouse_loc)
	{
		SDL = sdl;
		int mouse_vx, mouse_vy;
		std::tie(region_x, region_y) = region_loc;
		std::tie(mouse_vx, mouse_vy) = mouse_loc;

		screen_x = mouse_vx * 16 + 16;
		screen_y = mouse_vy * 16 + 48;

		// We are going to make a panel for each major option group
		info_windows.push_back(
				std::make_unique<terrain_info_window>(sdl,
						"Terrain Information", true, region_x, region_y));
		add_structure_panels(); // Settler panels are called from here
		info_windows.push_back(
				std::make_unique<build_options_window>(sdl, "Build Options",
						true, region_x, region_y));
	}

	bool render(const std::pair<int, int> mouse_loc, bool left_click,
			bool right_click)
	{
		int y = 90;
		int mouse_x, mouse_y;
		std::tie(mouse_x, mouse_y) = mouse_loc;

		bool escaping = false;
		bool in_any_window = false;

		for (unique_ptr<window> &sw : info_windows)
		{
			sw->location.x = 100;
			sw->location.y = y;
			if (sw->render(mouse_x, mouse_y, left_click))
				escaping = true;
			y += sw->location.h + 10;
			if (sw->in_window(mouse_x, mouse_y))
			{
				in_any_window = true;
			}
		}

		if (!in_any_window and count > 10 and (left_click or right_click))
			escaping = true;

		if (count < 15)
			++count;
		return escaping;
	}

private:
	sdl2_backend * SDL;
	int screen_x, screen_y, region_x, region_y;
	int idx;
	vector<std::unique_ptr<window>> info_windows;
	int count = 0;

	void add_structure_panels()
	{
		// Iterate components and find structures that are here
		line_buffer lines;

		vector<position_component *> positions = ECS->find_components_by_func<
				position_component>([this] ( const position_component &c )
		{
			return ( c.x == this->region_x and c.y == this->region_y );
		});

		for (const position_component * pos : positions)
		{
			settler_ai_component * settler = ECS->find_entity_component<
					settler_ai_component>(pos->entity_id);
			if (settler == nullptr)
			{
				// It's not a settler, so we can describe it here
				info_windows.push_back(
						std::make_unique<structure_info_window>(SDL, "", true,
								pos->entity_id));
			}
			else
			{
				info_windows.push_back(
						std::make_unique<settler_info_window>(SDL, "", true,
								settler));
			}
		}

	}

};

}

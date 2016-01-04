#pragma once

#include "window.h"
#include "../world/world.h"
#include "colors.h"
#include "../game.h"
#include "build_options_window.h"
#include "../raws/raw_input.h"
#include "../raws/raw_output.h"
#include "../raws/raw_power_drain.h"
#include "../raws/raw_emote.h"

namespace render
{

class structure_info_window: public window
{
private:
	vector<line_t> lines;
	int current_width = 0;
	int current_y = 8;

	void add_line(const string &s, const SDL_Color &col, const int &icon_idx =
			53, const bool &hotspot = false, std::function<bool()> func =
			do_nothing, const int &fill_lines = 0)
	{
		const pair<int, int> size = SDL->text_size("disco14", s);
		SDL_Rect tile_src = raws::get_tile_source(icon_idx);
		if (icon_idx == 0)
			tile_src.w = 0;
		line_t line
		{ s, col, tile_src, size.first, hotspot, func, fill_lines };
		if (size.first > current_width)
		{
			current_width = size.first;
		}
		lines.push_back(line);
	}
public:
	structure_info_window(sdl2_backend* sdl, const string& title,
			const bool& decor, const int &entity) :
			window(sdl, title, decor), entity_id(entity)
	{
		debug_name_component * name = ECS->find_entity_component<
				debug_name_component>(entity_id);
		if (name != nullptr)
		{
			window_title = name->debug_name;

			description_component * desc = ECS->find_entity_component<
					description_component>(entity_id);
			if (desc != nullptr)
			{
				renderable_component * renderable = ECS->find_entity_component<
						renderable_component>(entity_id);
				add_line(desc->desc, sdl_white, renderable->tile_idx);
			}

			// Render any held items
			vector<item_storage_component *> stored_items =
					ECS->find_components_by_func<item_storage_component>(
							[this] ( const item_storage_component &e )
							{
								if ( e.container_id == entity_id )
								{
									return true;
								}
								else
								{
									return false;
								}
							});
			for (item_storage_component * item : stored_items)
			{
				debug_name_component * nc = ECS->find_entity_component<
						debug_name_component>(item->entity_id);
				renderable_component * renderable = ECS->find_entity_component<
						renderable_component>(item->entity_id);
				if (!renderable->extra_tall)
				{
					add_line(string(" ") + nc->debug_name, sdl_green,
							renderable->tile_idx);
				}
				else
				{
					add_line(string(" ") + nc->debug_name, sdl_green, 0);
				}
				desc = ECS->find_entity_component<description_component>(
						item->entity_id);
				if (desc != nullptr)
				{
					add_line(string("  ") + desc->desc, sdl_white, 0);
				}
			}

			// Is it a tree? Trees are a special case
			tree_component * tree = ECS->find_entity_component<tree_component>(
					entity_id);
			if (tree != nullptr)
			{
				position_component * pos = ECS->find_entity_component<
						position_component>(entity_id);
				const int region_x = pos->x;
				const int region_y = pos->y;
				add_line("Chop Down Tree (Uses: Axe, Produces: Logs)",
						sdl_yellow, 56, true,
						[=]
						{
							game_engine->messaging->add_message<chop_order_message> ( chop_order_message ( region_x, region_y, entity_id, "Chop Tree" ) );
							return true;
						}, 1);
			}

			// It might also be a workshop that has reactions - determine this
			vector<raws::base_raw *> reactions =
					raws::get_possible_reactions_for_structure(
							name->debug_name);
			for (raws::base_raw * br : reactions)
			{
				vector<pair<string, int>> required_components;
				vector<pair<string, int>> creates;
				int power_drain = 0;
				string emote;
				string reaction_name;
				int output_idx = 0;

				for (const std::unique_ptr<raws::base_raw> &brc : br->children)
				{
					if (brc->type == raws::INPUT)
					{
						raws::raw_input * input =
								static_cast<raws::raw_input *>(brc.get());
						required_components.push_back(
								std::make_pair(input->required_item,
										input->quantity));
					}
					if (brc->type == raws::OUTPUT)
					{
						raws::raw_output * input =
								static_cast<raws::raw_output *>(brc.get());
						creates.push_back(
								std::make_pair(input->created_item,
										input->quantity));
					}
					if (brc->type == raws::POWER_DRAIN)
					{
						raws::raw_power_drain * input =
								static_cast<raws::raw_power_drain *>(brc.get());
						power_drain = input->quantity;
					}
					if (brc->type == raws::EMOTE)
					{
						raws::raw_emote * input =
								static_cast<raws::raw_emote *>(brc.get());
						emote = input->emote;
					}
					if (brc->type == raws::NAME)
					{
						reaction_name = brc->get_name_override();
					}
				}

				/*
				 std::cout << "REACTION -- " << reaction_name << ": \n";
				 for (std::pair<string,int> &tmp : required_components) {
				 std::cout << " <-- " << tmp.first << " x " << tmp.second << "\n";
				 }
				 for (std::pair<string,int> &tmp : creates) {
				 std::cout << " --> " << tmp.first << " x " << tmp.second << "\n";
				 }
				 std::cout << " <-- " << power_drain << " units of power.\n";
				 std::cout << " === " << emote << "\n";
				 std::cout << " Output tile idx: " << output_idx << "\n";
				 */

				bool possible = true;
				for (const std::pair<string, int> &component : required_components)
				{
					auto finder = world::inventory.find(component.first);
					if (finder == world::inventory.end())
					{
						possible = false;
					}
					else
					{
						const int available_components = finder->second.size();
						if (available_components < component.second)
							possible = false;
					}
				}

				if (possible)
				{
					add_line(reaction_name, sdl_yellow, output_idx, true,
							[=]
							{
								game_engine->messaging->add_message<reaction_order_message>( reaction_order_message( reaction_name, entity_id, required_components, creates, emote, power_drain ) );
								return true;
							}, 0);
				}
				else
				{
					std::cout << "Reaction " << reaction_name
							<< " was ruled impossible.\n";
				}
			}
		}

		// Render the extended line-buffer
		const int height = (lines.size() * 16) + 32;
		location =
		{	0, 0, current_width +32, height};

		allocate();
		render_decorations();

		SDL->texture_target(texture_id);

		const int x = 16;
		int y = 16;
		for (const line_t &panel_line : lines)
		{
			std::string line_s = SDL->render_text_to_image("disco14",
					panel_line.text, "tmp", panel_line.color);
			SDL->render_bitmap_simple(line_s, x, y);
			if (panel_line.icon_source.w != 0)
				SDL->render_bitmap("spritesheet", panel_line.icon_source,
						SDL_Rect
						{ 0, y, 16, 16 });

			if (panel_line.is_hotspot)
			{
				hotspots.push_back(
						{ SDL_Rect
						{ 0, y, current_width + 32, 16
								+ (panel_line.hotspot_lines * 16) },
								panel_line.callback });
			}

			y += 16;
		}

		SDL->reset_texture_target();
	}

private:
	const int entity_id;
};

}

#include "gui_render_system.h"
#include "../game.h"
#include "../world/universe.hpp"
#include "render_ascii.hpp"
#include "gui/gui_static_text.hpp"
#include <memory>
#include <sstream>

using std::stringstream;

void gui_render_system::render_cursor(engine::sdl2_backend * SDL, std::pair<int, int> &screen_size, SDL_Rect &viewport, position_component3d * camera_pos)
{
	int mouse_x = engine::command::mouse_x;
	int mouse_y = engine::command::mouse_y;

	if (mouse_x > 0 and mouse_x < screen_size.first and mouse_y > 48 and mouse_y < screen_size.second)
	{
		const int tile_x = mouse_x / 8;
		const int tile_y = (mouse_y - 48) / 8;

		const int16_t tilespace_x = tile_x + viewport.x;
		const int16_t tilespace_y = tile_y + viewport.y;

		if (tilespace_x == last_mouse_x and tilespace_y == last_mouse_y)
		{
			++mouse_hover_time;
		}
		else
		{
			mouse_hover_time = 0;
		}
		last_mouse_x = tilespace_x;
		last_mouse_y = tilespace_y;

		const location_t target	{ camera_pos->pos.region, tilespace_x, tilespace_y,	camera_pos->pos.z };
		const int target_idx = get_tile_index(tilespace_x, tilespace_y,	camera_pos->pos.z);

		engine::vterm::screen_character cursor{ 176, color_t{ 255, 255, 0 }, color_t{ 0, 0, 0 } };
		if (!world::planet->get_region(target.region)->revealed[target_idx])
		{
			cursor.foreground_color = color_t{ 64, 64, 64 };
		}
		else
		{
			tile_t * target_tile = world::planet->get_tile(target);
			if (target_tile->flags.test(TILE_OPTIONS::SOLID))
			{
				cursor.foreground_color = color_t{ 255, 0, 0 };
			}
			if (target_tile->flags.test(TILE_OPTIONS::CAN_STAND_HERE))
			{
				cursor.foreground_color = color_t{ 0, 255, 255 };
			}
		}

		int alpha = 32 + (mouse_hover_time * 8);
		if (alpha > 200) alpha = 200;
		SDL->set_alpha_mod("font_s", alpha);
		SDL_Rect dest{ tile_x * 8, (tile_y * 8) + 48, 8, 8 };
		render_ascii(dest, cursor, SDL, 0, make_tuple(1.0,1.0,1.0),true);
		SDL->set_alpha_mod("font_s", 255);

		if (world::planet->get_region(camera_pos->pos.region)->revealed[target_idx])
		{
			tile_t * target_tile = world::planet->get_tile(target);
			{
				stringstream ss;
				ss << tile_type_to_string(target_tile->base_tile_type) << " / ";
				if (target_tile->base_tile_type != tile_type::EMPTY_SPACE)
				{
					ss << ground_type_to_string(target_tile->ground) << " / ";
					ss << covering_type_to_string(target_tile->covering) << " / ";
				}
				ss << climate_type_to_string(target_tile->climate);

				gui_element * tooltip1 = gui.get_element("tooltip1");
				if (tooltip1 == nullptr) {
					gui.add_element("tooltip1", std::make_unique<gui_static_text>( "disco10", ss.str(), render::sdl_white, 10, screen_size.second-10 ));
				} else {
					gui_static_text * tt1 = static_cast<gui_static_text *>(tooltip1);
					tt1->message = ss.str();
				}

				//std::string line_s = SDL->render_text_to_image("disco10", ss.str(), "tmp", render::sdl_white);
				//SDL->render_bitmap_simple(line_s, 10, screen_size.second-10);
			}

			// Add tile contents
			stringstream ss;

			int region_x = tilespace_x;
			int region_y = tilespace_y;
			int region_z = camera_pos->pos.z;
			vector<position_component3d *> positions = ECS->find_components_by_func<position_component3d>([&ss, region_x, region_y, region_z] ( const position_component3d &c )
			{
				return ( c.pos.x == region_x and c.pos.y == region_y and c.pos.z == region_z );
			});
			for (const position_component3d * pos : positions)
			{
				const int entity_id = pos->entity_id;

				const settler_ai_component * settler = ECS->find_entity_component<settler_ai_component>(entity_id);
				if ( settler != nullptr) {
					ss << settler->first_name << " " << settler->last_name << "  ";
				} else {
					debug_name_component * name = ECS->find_entity_component<debug_name_component>(entity_id);
					if (name != nullptr) ss << name->debug_name << "  ";
				}

				vector<item_storage_component *> stored_items =	ECS->find_components_by_func<item_storage_component>(
									[entity_id] ( const item_storage_component &e )
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
						debug_name_component * nc = ECS->find_entity_component<debug_name_component>(item->entity_id);
						ss << "[" << nc->debug_name << "]  ";
					}
			}

			if (ss.str().size() < 1) {
				ss << "Nothing interesting here.";
			}
			gui_element * tooltip2 = gui.get_element("tooltip2");
			if (tooltip2 == nullptr) {
				gui.add_element("tooltip2", std::make_unique<gui_static_text>("disco10", ss.str(), render::sdl_white, 10, screen_size.second+2));
			} else {
				gui_static_text * tt2 = static_cast<gui_static_text *>(tooltip2);
				tt2->message = ss.str();
			}

		}
	}
}

void gui_render_system::tick(const double& duration_ms) {
	sdl2_backend * SDL = game_engine->get_backend();

	// Calculations - these are duplicated in map_render and should be refactored to be done once.
	position_component3d * camera_pos = get_camera_position();
	region_t * current_region = world::planet->get_region(camera_pos->pos.region);
	pair<int, int> screen_size = SDL->get_screen_size();
	screen_size.second -= 48;
	const int ascii_width = screen_size.first / 8;
	const int ascii_height = screen_size.second / 8;
	if (camera_pos->pos.x < ascii_width / 2) camera_pos->pos.x = ascii_width / 2;
	if (camera_pos->pos.x > REGION_WIDTH - (ascii_width / 2)) camera_pos->pos.x = REGION_WIDTH - (ascii_width / 2);
	if (camera_pos->pos.y < ascii_height / 2) camera_pos->pos.y = ascii_height / 2;
	if (camera_pos->pos.y > REGION_HEIGHT - (ascii_height / 2))	camera_pos->pos.y = REGION_HEIGHT - (ascii_height / 2);
	SDL_Rect viewport{ camera_pos->pos.x - (ascii_width / 2), camera_pos->pos.y	- (ascii_height / 2), ascii_width, ascii_height };

	// Render mode
	if (mode == normal and engine::command::right_click and mouse_x > 0 and mouse_x < screen_size.first and mouse_y > 48 and mouse_y < screen_size.second) {
		mode = radial;
		const int tile_x = mouse_x / 8;
		const int tile_y = (mouse_y - 48) / 8;

		const int16_t tilespace_x = tile_x + viewport.x;
		const int16_t tilespace_y = tile_y + viewport.y;

		radial_screen_x = mouse_x;
		radial_screen_y = mouse_y;
		radial_tilespace_x = tilespace_x;
		radial_tilespace_y = tilespace_y;
		radial_tilespace_z = camera_pos->pos.z;

		universe->globals.paused = true;

		// TODO: Launch the pop-ups!
	}

	if (mode == normal)
	{
		render_cursor(SDL, screen_size, viewport, camera_pos);
	}
	else if (mode == radial)
	{
		if (!universe->globals.paused)
		{
			mode = normal;
		}
	}
	gui.render();
}

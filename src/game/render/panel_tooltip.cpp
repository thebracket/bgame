#include "panel_tooltip.h"
#include "../world/planet.hpp"

namespace render
{

panel_tooltip::panel_tooltip(sdl2_backend * sdl, const location_t &loc,
		const std::pair<int, int> mouse_loc)
{
	SDL = sdl;
	int region_x, region_y, region_z, mouse_vx, mouse_vy;
	region_x = loc.x;
	region_y = loc.y;
	region_z = loc.z;
	std::tie(mouse_vx, mouse_vy) = mouse_loc;

	/*const int idx = world::current_region->idx ( region_x, region_y );
	 if ( world::current_region->visible[ idx ] == false ) {
	 render_me = false;
	 return;
	 }*/

	screen_x = mouse_vx * 8 + 8;
	screen_y = mouse_vy * 8 + 48;

	lines.add_line(SDL, "Right Click for Options", sdl_red);
	tile_t * target_tile = world::planet->get_tile(loc);
	stringstream ss;
	ss << tile_type_to_string(target_tile->base_tile_type) << " / ";
	if (target_tile->base_tile_type != tile_type::EMPTY_SPACE)
	{
		ss << ground_type_to_string(target_tile->ground) << " / ";
		ss << covering_type_to_string(target_tile->covering) << " / ";
	}
	ss << climate_type_to_string(target_tile->climate);

	lines.add_line(SDL, ss.str(), sdl_magenta);
	add_tile_contents(loc.x, loc.y, loc.z);

	render_buffer();
}

void panel_tooltip::render(const int hover_time)
{
	const int width = std::min(panel_holder->width, hover_time * 16);
	const int height = std::min(panel_holder->height, hover_time * 4);
	SDL_Rect source
	{ 0, 0, width, height };
	int line_size = static_cast<int>((lines.size() / 2) * 16);
	SDL_Rect dest
	{ screen_x, screen_y - line_size, width, height };
	SDL->render_bitmap(panel_holder->texture_id, source, dest);
}

void panel_tooltip::render_buffer()
{
	const int width = lines.get_width() + 32;
	const int height = (lines.size() * 16) + 8;
	panel_holder = std::make_unique<panel>(SDL, width, height);
	SDL->texture_target(panel_holder->texture_id);

	SDL_Rect source = raws::get_tile_source_by_name("BLACKMASK");
	SDL_Rect dest
	{ 0, 0, width, height };
	SDL->render_bitmap("spritesheet", source, dest);

	int y = 8;
	const int x = 16;
	for (const std::pair<std::string, SDL_Color> &tooltip_line : lines.lines)
	{
		std::string line_s = SDL->render_text_to_image("disco14",
				tooltip_line.first, "tmp", tooltip_line.second);
		SDL->render_bitmap_simple(line_s, x, y);
		y += 16;
	}

	SDL->reset_texture_target();
}

void panel_tooltip::add_tile_contents(const int region_x, const int region_y,
		const int region_z)
{
	vector<position_component3d *> positions =
			ECS->find_components_by_func<position_component3d>(
					[region_x, region_y, region_z] ( const position_component3d &c )
					{
						return ( c.pos.x == region_x and c.pos.y == region_y and c.pos.z == region_z );
					});
	for (const position_component3d * pos : positions)
	{
		const int entity_id = pos->entity_id;
		if (!add_settler_details(entity_id))
			add_name_details(entity_id);
		add_containers(entity_id);
	}
}

bool panel_tooltip::add_settler_details(const int entity_id)
{
	const settler_ai_component * settler = ECS->find_entity_component<
			settler_ai_component>(entity_id);
	if (settler == nullptr)
		return false;

	stringstream settler_name;
	settler_name << settler->first_name << " " << settler->last_name << " ("
			<< settler->profession_tag << ")";
	lines.add_line(SDL, settler_name.str(), render::sdl_yellow);

	return true;
}

void panel_tooltip::add_name_details(const int entity_id)
{
	const debug_name_component * name = ECS->find_entity_component<
			debug_name_component>(entity_id);
	if (name != nullptr)
	{
		lines.add_line(SDL, name->debug_name, render::sdl_white);
	}
}

void panel_tooltip::add_containers(const int entity_id)
{
	vector<item_storage_component *> stored_items =
			ECS->find_components_by_func<item_storage_component>(
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
		debug_name_component * nc = ECS->find_entity_component<
				debug_name_component>(item->entity_id);
		lines.add_line(SDL, string(" ") + nc->debug_name, render::sdl_green);
	}
}

}

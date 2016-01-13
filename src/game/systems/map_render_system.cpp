#include "map_render_system.h"
#include "../game.h"
#include "../world/universe.hpp"
#include "../render/colors.h"
#include "../../engine/virtual_terminal.hpp"
#include "../world/tile_types.hpp"
#include "../render/render_list.hpp"
#include "render_ascii.hpp"
#include <tuple>

using namespace engine;
using namespace engine::command;
using engine::vterm::color_t;
using std::make_unique;
using engine::vterm::desaturate;
using engine::vterm::darken;
using engine::vterm::apply_colored_light;
using std::make_tuple;

void render_emotes(sdl2_backend * SDL, SDL_Rect &viewport)
{

	vector<chat_emote_message> * emote_ptr =
			game_engine->messaging->get_messages_by_type<chat_emote_message>();
	for (chat_emote_message &emote : *emote_ptr)
	{
		//std::cout << "Emote: " << emote.message << "\n";
		const unsigned char fade = 4 * emote.ttl;
		const SDL_Color sdl_black
		{ 0, 0, 0, 0 };
		SDL_Color text_color = sdl_black;
		switch (emote.color)
		{
		case BLACK:
			text_color = render::sdl_black;
			break;
		case WHITE:
			text_color = render::sdl_white;
			break;
		case YELLOW:
			text_color = render::sdl_yellow;
			break;
		case CYAN:
			text_color = render::sdl_cyan;
			break;
		case GREEN:
			text_color = render::sdl_green;
			break;
		case MAGENTA:
			text_color = render::sdl_magenta;
			break;
		case RED:
			text_color = render::sdl_red;
			break;
		case BLUE:
			text_color = render::sdl_blue;
			break;
		}
		string emote_text = SDL->render_text_to_image("disco12",
				emote.message, "tmp", sdl_black);
		SDL->set_alpha_mod(emote_text, fade);
		SDL->set_alpha_mod("emote_bubble", fade);
		std::pair<int, int> emote_size = SDL->query_bitmap_size(emote_text);

		// Left part of bubble
		const int x = (emote.x - viewport.x) * 8;
		const int y = (emote.y - viewport.y) * 8 + 48;
		const int height = emote_size.second;

		SDL_Rect src
		{ 0, 0, 4, 8 };
		SDL_Rect dest
		{ x, y, 4, height };
		SDL->render_bitmap("emote_bubble", src, dest);

		// Center of bubble
		src =
		{	5, 0, 4, 8};
		dest =
		{	x + 4, y, emote_size.first, height};
		SDL->render_bitmap("emote_bubble", src, dest);

		// Right part of bubble
		src =
		{	27, 0, 4, 8};
		dest =
		{	x + 4+emote_size.first, y, 4, height};
		SDL->render_bitmap("emote_bubble", src, dest);

		// The text itself
		SDL->render_bitmap_simple(emote_text, x + 4, y);
		SDL->set_alpha_mod("emote_bubble", 0);
	}

	vector<highlight_message> * highlights =
			game_engine->messaging->get_messages_by_type<highlight_message>();
	for (highlight_message &highlight : *highlights)
	{
		SDL_Rect dest
		{ (highlight.tile_x - viewport.x) * 8, ((highlight.tile_y
				- viewport.y) * 8) + 48, 8, 8 };
		engine::vterm::screen_character highlight_c
		{ 219, color_t
		{ 255, 0, 255 }, color_t
		{ 0, 0, 0 } };
		SDL->set_alpha_mod("font_s", 128 + highlight.ttl);
		render_ascii(dest, highlight_c, SDL, 0, make_tuple(1.0,1.0,1.0),true);
	}
	SDL->set_alpha_mod("font_s", 255);
}

void render_particles(sdl2_backend * SDL, SDL_Rect &viewport)
{
	vector<particle_message> * particles =
			game_engine->messaging->get_messages_by_type<particle_message>();
	for (particle_message &particle : *particles)
	{
		//std::cout << "Particle!\n";
		SDL_Color text_color = render::sdl_black;
		switch (particle.color)
		{
		case WHITE:
			text_color = render::sdl_light_grey;
			break;
		case YELLOW:
			text_color = render::sdl_yellow;
			break;
		case CYAN:
			text_color = render::sdl_cyan;
			break;
		case GREEN:
			text_color = render::sdl_green;
			break;
		case MAGENTA:
			text_color = render::sdl_magenta;
			break;
		case RED:
			text_color = render::sdl_red;
			break;
		case BLUE:
			text_color = render::sdl_blue;
			break;
		case BLACK:
			text_color = render::sdl_dark_grey;
			break;
		}
		string emote_text = SDL->render_text_to_image("disco12",
				particle.message, "tmp", text_color);
		std::pair<int, int> emote_size = SDL->query_bitmap_size(emote_text);
		const int x = (particle.tile_x - viewport.x) * 8;
		const int y = (particle.tile_y - viewport.y - 1) * 8 + 48 + 4;
		SDL_Rect src
		{ 0, 0, emote_size.first, emote_size.second };
		SDL_Rect dst
		{ x + particle.offset_x, y + particle.offset_y, particle.ttl / 4,
				particle.ttl / 4 };
		SDL->render_bitmap(emote_text, src, dst);
	}
}

void map_render_system::tick(const double& duration_ms) {
	sdl2_backend * SDL = game_engine->get_backend();

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

	for (int y = 0; y < viewport.h; ++y)
	{
		for (int x = 0; x < viewport.w; ++x)
		{
			SDL_Rect dest
			{ x * 8, (y * 8) + 48, 8, 8 };
			vterm::screen_character target
			{ '.', color_t
			{ 255, 255, 255 }, color_t
			{ 0, 0, 0 } };
			const location_t world_loc
			{ camera_pos->pos.region, static_cast<uint8_t>(viewport.x + x),
					static_cast<uint8_t>(viewport.y + y), camera_pos->pos.z };
			tile_t * tile = world::planet->get_tile(world_loc);

			const int tile_idx = get_tile_index(world_loc.x, world_loc.y,
					world_loc.z);

			if (!tile->flags.test(TILE_OPTIONS::SOLID)
					and tile->base_tile_type == tile_type::EMPTY_SPACE
					and !render_list_3d[tile_idx])
			{
				// 3d dive
				int depth = 1;
				bool go = false;
				int dive_tile_idx;
				while (!go)
				{
					tile_t * dive_tile = world::planet->get_tile(
							location_t
							{ camera_pos->pos.region, world_loc.x,
									world_loc.y,
									static_cast<uint8_t>(camera_pos->pos.z
											- depth) });
					dive_tile_idx = get_tile_index(world_loc.x, world_loc.y,
							world_loc.z - depth);

					if (!dive_tile->flags.test(TILE_OPTIONS::SOLID)
							and dive_tile->base_tile_type
									== tile_type::EMPTY_SPACE
							and !render_list_3d[dive_tile_idx])
					{
						++depth;
						if (depth > 10)
							go = true;
					}
					else
					{
						if (dive_tile->flags.test(TILE_OPTIONS::SOLID))
						{
							target.character = 219;
							target.foreground_color = color_t
							{ 128, 128, 128 };
							target.background_color = color_t
							{ 0, 0, 0 };
						}
						else
						{
							target = dive_tile->render_as;
							go = true;
						}

						if (render_list_3d[dive_tile_idx])
						{
							target = render_list_3d[dive_tile_idx].value();
						}
					}
				}
				//std::cout << "Dive reached depth: " << depth << "\n";
				if (depth < 10 and (current_region->revealed[dive_tile_idx]	or universe->globals.omniscience))
				{
					render_ascii(dest, target, SDL, depth, current_region->tiles[dive_tile_idx].light_color, current_region->visible[dive_tile_idx]);
				}

			}
			else
			{
				if (tile->flags.test(TILE_OPTIONS::SOLID))
				{
					target.character = 219;
					target.foreground_color = color_t{ 128, 128, 128 };
					target.background_color = color_t{ 0, 0, 0 };
				}
				else
				{
					target = tile->render_as;
				}

				if (render_list_3d[tile_idx])
				{
					target = render_list_3d[tile_idx].value();
				}
				if (current_region->revealed[tile_idx] or universe->globals.omniscience)
				{
					render_ascii(dest, target, SDL, 0, current_region->tiles[tile_idx].light_color,	current_region->visible[tile_idx]);
				}

			}
		} // x
	} // y

	render_emotes(SDL, viewport);
	render_particles(SDL, viewport);
}

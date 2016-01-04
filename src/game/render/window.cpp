#include "window.h"
#include "colors.h"
#include <iostream>

namespace render
{

window::~window()
{
	if (!texture_id.empty())
	{
		SDL->mark_texture_for_deletion(texture_id);
	}
}

bool window::render(const int& mouse_x, const int& mouse_y, const bool clicked)
{
	bool escaping = false;
	SDL_Rect src
	{ 0, 0, location.w, location.w };
	SDL->render_bitmap(texture_id, src, location);

	for (const hotspot &hs : hotspots)
	{
		const int hx = hs.area.x + location.x;
		const int hy = hs.area.y + location.y;
		if (mouse_x >= hx and mouse_x < (hx + hs.area.w) and mouse_y >= hy
				and mouse_y < (hy + hs.area.h))
		{
			// Highlight
			SDL_Rect dest
			{ hx, hy, hs.area.w, hs.area.h };
			src = raws::get_tile_source_by_name("WINDOWMASK");
			SDL->set_alpha_mod("spritesheet", 128);
			SDL->set_color_mod("spritesheet", 128, 0, 0);
			SDL->render_bitmap("spritesheet", src, dest);
			SDL->set_alpha_mod("spritesheet", 255);
			SDL->set_color_mod("spritesheet", 255, 255, 255);

			// Click handler
			if (clicked)
				escaping = hs.on_click();
		}
	}

	return escaping;
}

void window::allocate()
{
	texture_id = SDL->create_temporary_texture(location.w, location.h);
}

void window::render_decorations()
{
	SDL->texture_target(texture_id);
	SDL->clear_screen();

	SDL_Rect source = raws::get_tile_source_by_name("WINDOWMASK");
	SDL_Rect dest
	{ 0, 0, location.w, location.h };
	SDL->render_bitmap("spritesheet", source, dest);

	if (render_decor)
	{
		source = raws::get_tile_source_by_name("WINDOWHEAD");
		dest =
		{	0, 0, location.w, 16};
		SDL->render_bitmap("spritesheet", source, dest);

		const string title_texture = SDL->render_text_to_image("disco14",
				window_title, "tmp", sdl_white);
		SDL->render_bitmap_centered(title_texture,
				location.x + (location.w / 2), location.y + 1);
	}

	SDL->reset_texture_target();
}

}

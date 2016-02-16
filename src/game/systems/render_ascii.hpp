#pragma once

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

inline void render_ascii(const SDL_Rect &dest, const vterm::screen_character &target, sdl2_backend * SDL,
			int depth, const std::tuple<float,float,float> &light_color, bool visible)
{
	unsigned char target_char = target.character;
	int texture_x = (target_char % 16) * 8;
	int texture_y = (target_char / 16) * 8;
	SDL_Rect source{ texture_x, texture_y, 8, 8 };
	//const SDL_Rect bg_source = { 88, 104, 8, 8 };

	color_t fg = target.foreground_color;
	if (depth > 0)
	{
		const int darken_amount = depth * 25;
		darken(darken_amount, fg);
	}

	if (!visible and !universe->globals.omniscience)
	{
		desaturate(fg);
	}

	// Apply lighting
	apply_colored_light(fg, light_color);

	SDL->set_color_mod("font_s", std::get<0>(fg), std::get<1>(fg), std::get<2>(fg));
	if (target.rotation != 0) {
		SDL->render_bitmap_rotated("font_s", source, dest, target.rotation);
	} else {
		SDL->render_bitmap("font_s", source, dest);
	}
}

#include "map_render_system.hpp"
#include "../../messages/messages.hpp"

using namespace rltk;
using namespace map_render_sys;

uint32_t glyph_cycle = 0;

namespace map_render_sys {
	bool mouse_in_terminal = false;
	int mouse_term_x = 0;
	int mouse_term_y = 0;
	bool building_possible = false;
}

/*
vchar greyscale(vchar target) {
	const color_t grey_fg = greyscale(target.foreground);
	const color_t grey_bg = greyscale(target.background);

	target.foreground = lerp(target.foreground, grey_fg, 0.7F);
	target.background = lerp(target.background, grey_bg, 0.5F);
	return target;
}

float get_fraction(const float &f) {
    double ignore_me;
    return std::modf(f, &ignore_me);
}

vchar get_render_char(const int &x, const int &y) {
	const int idx = render_tiles[((term(1)->term_width * y) + x)];
    //int world_x, world_y, world_z;
    //std::tie(world_x, world_y, world_z) = idxmap(idx);

	if (idx == 0) return vchar{' ', rltk::colors::BLACK, rltk::colors::BLACK};
	if (!current_region->revealed[idx]) return vchar{' ', rltk::colors::BLACK, rltk::colors::BLACK};

    if (current_region->veg_cache[idx].glyph != 0) {
        rltk::vchar plant = current_region->veg_cache[idx];
        sterm(6)->add(xchar{static_cast<int>(plant.glyph), lerp(plant.foreground, light_map[((term(1)->term_width * y) + x)], 0.75), static_cast<float>(x), static_cast<float>(y+2)});
    }

    // Apply blood stains!
    if (current_region->blood_stains[idx]) {
        sterm(5)->add(xchar{
                static_cast<int>(340),
                rltk::colors::WHITE,
                (float)x,
                (float)y+2.0F});
    }

    // Apply regular render items
    vchar result{' ', rltk::colors::GREY, rltk::colors::BLACK};
    result = current_region->render_cache[idx];
    if (!current_region->visible[idx]) result = greyscale(result);
	auto rf = renderables.find(idx);
	if (rf != renderables.end()) {
		screen_render_t renderable = rf->second[glyph_cycle % rf->second.size()];
        sterm(5)->add(xchar{
				static_cast<int>(renderable.c.glyph),
                lerp(renderable.c.foreground, light_map[((term(1)->term_width * y) + x)], 0.75),
                static_cast<float>(x + renderable.offsetX),
                static_cast<float>(y+2 + renderable.offsetY)});
	}

    // Apply composite render - settlers
	auto crf = composite_renderables.find(idx);
    if (crf != composite_renderables.end()) {
        for (const screen_render_t &vc : crf->second[glyph_cycle % crf->second.size()]) {
            sterm(5)->add(xchar{
                    static_cast<int>(vc.c.glyph),
                    vc.c.foreground,
                    (float)x+vc.offsetX,
                    (float)y+2.0F+vc.offsetY});
        }
    }

	// Apply lighting
	result.foreground = lerp(result.foreground, light_map[((term(1)->term_width * y) + x)], 0.75);

	const int visible_z = idx / ( REGION_WIDTH * REGION_HEIGHT);
	if (visible_z == camera_position->region_z) {
		return result;
	} else {
		const int darken_amount = (camera_position->region_z - visible_z) * 40;
		vchar darkened = result;
		darkened.foreground = darken(darken_amount, darkened.foreground);
		darkened.background = darken(darken_amount, darkened.background);		
		return darkened;
	}
}

vchar get_render_char_mining(const int &x, const int &y) {

	vchar result = get_render_char(x,y);
	const int idx = render_tiles[((term(1)->term_width * y) + x)];

	auto mf = designations->mining.find(idx);
	if (mf != designations->mining.end()) {
		result.foreground = rltk::colors::YELLOW;
		result.background = rltk::colors::GREY;
		switch (mf->second) {
			case 1 : result.glyph = 177; break;
			case 2 : result.glyph = 31; break;
			case 3 : result.glyph = 30; break;
			case 4 : result.glyph = '<'; break;
			case 5 : result.glyph = '>'; break;
			case 6 : result.glyph = 'X'; break;
		}
	}

	return result;
}

vchar get_render_char_chopping(const int &x, const int &y) {

	vchar result = get_render_char(x,y);
	const auto idx = render_tiles[((term(1)->term_width * y) + x)];

	const auto tree_id = current_region->tree_id[idx];
	if (tree_id > 0) {
		auto mf = designations->chopping.find((int)tree_id);
		if (mf != designations->chopping.end()) {
			result.foreground = rltk::colors::BLACK;
			result.background = rltk::colors::RED;
			result.glyph = '*';
		}
	}

	return result;
}

vchar get_render_char_guarding(const int &x, const int &y) {

	vchar result = get_render_char(x,y);
	const int idx = render_tiles[((term(1)->term_width * y) + x)];

	for (const auto &g : designations->guard_points) {
		if (mapidx(g.second) == idx) {
			result.foreground = rltk::colors::CYAN;
			result.background = rltk::colors::BLACK;
			result.glyph = 'G';
		}
	}

	return result;
}

vchar get_render_char_harvest(const int &x, const int &y) {

	vchar result = get_render_char(x,y);
	const int idx = render_tiles[((term(1)->term_width * y) + x)];

	for (const auto &g : designations->harvest) {
		if (mapidx(g.second) == idx) {
			result.foreground = rltk::colors::CYAN;
			result.background = rltk::colors::BLACK;
			result.glyph = 'H';
		}
	}

	return result;
}

vchar get_render_char_stockpiles(const int &x, const int &y) {

    vchar result = get_render_char(x,y);
    const int idx = render_tiles[((term(1)->term_width * y) + x)];

    if (current_stockpile > 0 && current_region->stockpile_id[idx]==current_stockpile) {
        result.foreground = rltk::colors::MAGENTA;
    }

    return result;
}

vchar get_render_char_architecture(const int &x, const int &y) {

    vchar result = get_render_char(x,y);
    const int idx = render_tiles[((term(1)->term_width * y) + x)];

    auto mf = designations->architecture.find(idx);
    if (mf != designations->architecture.end()) {
        result.foreground = rltk::colors::YELLOW;
        result.background = rltk::colors::GREY;
        switch (mf->second) {
            case 0 : result.glyph = '#'; break;
            case 1 : result.glyph = '.'; break;
            case 2 : result.glyph = '<'; break;
            case 3 : result.glyph = '>'; break;
            case 4 : result.glyph = 'X'; break;
            case 5 : result.glyph = '^'; break;
            case 6 : result.glyph = '='; break;
        }
    }

    if (mouse_in_terminal) {
        arch_x = mouse_term_x;
        arch_y = mouse_term_y;
    }

    if (arch_available && mouse_in_terminal
        && designations->architecture.find(mapidx(mouse_term_x, mouse_term_y, camera_position->region_z))==designations->architecture.end())
    {
        const int building_left_x = mouse_term_x;
        const int building_top_y = mouse_term_y;
        const int building_right_x = mouse_term_x + arch_width;
        const int building_bottom_y = mouse_term_y + arch_height;

        if (arch_filled) {
            if (x + clip_left >= building_left_x && x + clip_left < building_right_x &&
                y + clip_top >= building_top_y && y + clip_top < building_bottom_y) {
                result.background = rltk::colors::BLACK;
                result.glyph = 177;

                if (arch_possible && !current_region->solid[idx] &&
                    !current_region->tile_flags[idx].test(CONSTRUCTION) && current_region->bridge_id[idx]==0) {
                    result.foreground = rltk::colors::GREEN;
                } else {
                    arch_possible = false;
                    result.foreground = rltk::colors::RED;
                }
            }
        } else {
            if (x + clip_left >= building_left_x && x + clip_left < building_right_x &&
                y + clip_top >= building_top_y && y + clip_top < building_bottom_y) {
                bool interior = false;
                if (arch_width > 2 && x+clip_left >= building_left_x+1 && x+clip_left < building_right_x-1 ) {
                    interior = true;
                }
                if (arch_height > 2 && y+clip_top >= building_top_y+1 && y+clip_top < building_bottom_y-1) {
                    interior = true;
                }
                if (x+clip_left == building_left_x) interior=false;
                if (x+clip_left == building_right_x-1) interior = false;
                if (y+clip_top == building_top_y) interior = false;
                if (y+clip_top == building_bottom_y-1) interior = false;

                if (!interior) {
                    result.background = rltk::colors::BLACK;
                    result.glyph = 177;

                    if (arch_possible && !current_region->solid[idx] &&
                        !current_region->tile_flags[idx].test(CONSTRUCTION) && current_region->bridge_id[idx]==0) {
                        result.foreground = rltk::colors::GREEN;
                    } else {
                        arch_possible = false;
                        result.foreground = rltk::colors::RED;
                    }
                }
            }
        }
    }

    return result;
}

vchar get_render_char_building(const int &x, const int &y) {

	vchar result = get_render_char(x,y);
	const int idx = render_tiles[((term(1)->term_width * y) + x)];

	// Build designations go here
	if (has_build_mode_building && mouse_in_terminal) {
		int offset_x = 0;
		int offset_y = 0;

		if (build_mode_building.width == 3) offset_x = -1;
		if (build_mode_building.height == 3) offset_y = -1;

		const int building_left_x = mouse_term_x + offset_x;
		const int building_top_y = mouse_term_y + offset_y;
		const int building_right_x = mouse_term_x + build_mode_building.width + offset_x;
		const int building_bottom_y = mouse_term_y + build_mode_building.height + offset_y;
		if (x+clip_left >= building_left_x && x+clip_left < building_right_x && y+clip_top>= building_top_y && y+clip_top < building_bottom_y) {
			result.background = rltk::colors::BLACK;
			result.glyph = 177;

			if ((!current_region->solid[idx] && current_region->tile_flags[idx].test(CAN_STAND_HERE)
				&& !current_region->tile_flags[idx].test(CONSTRUCTION) 
				&& !(current_region->tile_type[idx] == tile_type::STAIRS_DOWN)
				&& !(current_region->tile_type[idx] == tile_type::STAIRS_UP)
				&& !(current_region->tile_type[idx] == tile_type::STAIRS_UPDOWN))
				|| (build_mode_building.tag == "floor"
					&& !current_region->tile_flags[idx].test(CONSTRUCTION)&& !(current_region->tile_type[idx] == tile_type::STAIRS_DOWN)
					&& !(current_region->tile_type[idx] == tile_type::STAIRS_UP)
					&& !(current_region->tile_type[idx] == tile_type::STAIRS_UPDOWN)
					&& ( current_region->tile_flags[idx].test(CAN_STAND_HERE) || 
						current_region->tile_flags[idx-1].test(CAN_STAND_HERE) ||
						current_region->tile_flags[idx+1].test(CAN_STAND_HERE) ||
						current_region->tile_flags[idx-REGION_WIDTH].test(CAN_STAND_HERE) ||
						current_region->tile_flags[idx+REGION_WIDTH].test(CAN_STAND_HERE) 
					)
				)
			) {
				result.foreground = rltk::colors::GREEN;
			} else {
				result.foreground = rltk::colors::RED;
				building_possible = false;
			}
		}
	}

	return result;
}*/

void map_render_system::configure() {
	system_name = "Map Render";
	renderables_changed = true;
	dirty = true;
	subscribe_mbox<key_pressed_t>();
	subscribe_mbox<map_rerender_message>();
	subscribe<slow_tick_message>([this](slow_tick_message &msg) {
        ++glyph_cycle;
    });
}

void map_render_system::update(const double duration_ms) {
	// Handle re-render queues
	std::queue<map_rerender_message> * map_change = mbox<map_rerender_message>();
	while (!map_change->empty()) {
		dirty = true;
		map_change->pop();
	}

	return;

	/*
	int mouse_x, mouse_y;
	int font_w, font_h;
	std::tie(mouse_x, mouse_y) = get_mouse_position();
	std::tie(font_w, font_h) = term(1)->get_font_size();

	mouse_in_terminal = false;
	if (mouse_x > layer(1)->x && mouse_y > layer(1)->y && mouse_x < layer(1)->x+layer(1)->w && mouse_y < layer(1)->y + layer(1)->h ) {
		mouse_in_terminal = true;
		mouse_term_x = (mouse_x / font_w) + clip_left;
		mouse_term_y = (mouse_y / font_h) + clip_top - 2;
	}	

	if (dirty) {
		term(1)->clear();
        sterm(5)->clear();
        sterm(6)->clear();

		std::function<vchar(int,int)> calculator;
		if (game_master_mode == PLAY) {
			calculator = get_render_char;
		} else if (game_master_mode == ROGUE) {
			calculator = get_render_char;
		} else {
			switch (game_design_mode) {
				case DIGGING : calculator = get_render_char_mining; break;
				case CHOPPING : calculator = get_render_char_chopping; break;
				case BUILDING : { building_possible = true; calculator = get_render_char_building; } break;
				case GUARDPOINTS : calculator = get_render_char_guarding; break;
                case STOCKPILES : calculator = get_render_char_stockpiles; break;
                case HARVEST : calculator = get_render_char_harvest; break;
                case ARCHITECTURE : calculator = get_render_char_architecture; break;
			}
		}

		int Y = 2;
		for (int y=clip_top; y<clip_bottom; ++y) {
			int X = 0;
			for (int x=clip_left; x<clip_right; ++x) {
				rltk::vchar render_target = calculator(X, Y-2);
				term(1)->set_char(X, Y, render_target);
				++X;
			}
			++Y;
		}

		dirty = false;
		if (game_master_mode == DESIGN && game_design_mode == BUILDING) {
			if (mouse_in_terminal && (mouse_x / mouse::font3_w) > term(3)->term_width-30) mouse_in_terminal = false;
			dirty = true;
			if (get_mouse_button_state(rltk::button::LEFT) && mouse_in_terminal && building_possible && mouse_y / font_h>3) {
				emit(build_request_message{mouse_term_x, mouse_term_y, camera_position->region_z, build_mode_building});
				emit(refresh_available_buildings_message{});
				emit(map_dirty_message{});
				emit(renderables_changed_message{});
			}
		}
		renderables_changed = false;
	}*/
}

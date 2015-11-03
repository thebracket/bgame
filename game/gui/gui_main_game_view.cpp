#include "gui_main_game_view.h"
#include <iostream>
#include "../../engine/globals.h"
#include <sstream>
#include "../messages/chat_emote_message.h"

using std::stringstream;

namespace engine {

void gui_main_game_view::render_heading( const screen_region &viewport, const int &vp_left, const int &vp_right, const int &vp_top, const int &vp_bottom )
{
    // Heading
    constexpr char * heading{"<<< CORDEX OS 0.98 >>>"};
    constexpr color_t colors[] {
      { 0, 128, 128},
      { 0, 139, 139},
      { 0, 150, 150},
      { 0, 161, 161},
      { 0, 172, 172},
      { 0, 183, 183},
      { 0, 194, 194},
      { 0, 205, 205},
      { 0, 216, 216},
      { 0, 227, 227},
      { 0, 255, 255},
      { 0, 255, 255},
      { 0, 227, 227},
      { 0, 216, 216},
      { 0, 205, 205},
      { 0, 194, 194},
      { 0, 183, 183},
      { 0, 172, 172},
      { 0, 161, 161},
      { 0, 150, 150},
      { 0, 139, 139},
      { 0, 128, 128}
    };
    const int heading_x = viewport.w/2 - 11;
    //vterm::print(0, 0, "Cordex OS 0.9B", color_t{255,255,255}, color_t{0,0,0});
    for (std::size_t i=0; i<viewport.w+1; ++i) vterm::set_char_xy(i,0,{' ', black, color_t{0,128,128}});
    for (std::size_t i=0; i<22; ++i) {
	string character;
	character += heading[i];
	vterm::print(i+heading_x, 0, character, black, colors[i]);
    }
    
    // Display date/time/paused
    vterm::print(0,0,world::display_day_month,white,color_t{0,128,128});
    vterm::print(viewport.w - 4,0,world::display_time,white,color_t{0,128,128});
    if (world::paused) {
	vterm::print(viewport.w - 11, 0, "PAUSED", yellow, color_t{0,128,128});
    }
    
    // Display power levels
    stringstream ss;
    ss << "Power: " << world::stored_power << " / " << world::max_power;
    const string power_str = ss.str();
    const int length = power_str.size();
    const int power_x = (viewport.w/2) - (length/2);
    const int max_power_width = viewport.w+1;
    const float power_percent = float(world::stored_power) / float(world::max_power);
    float ticks = max_power_width * power_percent;
    const vterm::screen_character display_good {
        219, dark_green, black
    };
    const vterm::screen_character display_bad {
        176, red, black
    };
    for (int i=0; i<max_power_width; i++) {
        if (i <= ticks) {
            vterm::set_char_xy(i, 1, display_good);
        } else {
            vterm::set_char_xy(i, 1, display_bad);
        }
    }
    vterm::print(power_x, 1, power_str, yellow, black);
    
    // Display chat/emotes
    vector<chat_emote_message> * emote_ptr = engine::globals::messages->get_messages_by_type<chat_emote_message>();
    for (chat_emote_message &emote : *emote_ptr) {
	if (emote.x > vp_left and emote.x < vp_right and emote.y > vp_top and emote.y < vp_bottom) {
	    const int size = emote.message.size();
	    int emote_x = emote.x - vp_left;
	    const int emote_y = emote.y - vp_top+2;
	    if (emote.x + size > vp_right) emote.x -= (vp_right - size);
	    
	    const color_t emote_back{0, emote.ttl*8, emote.ttl*8};
	    vterm::set_char_xy(emote_x, emote_y, {16, black, emote_back});
	    vterm::print(emote_x+1, emote_y, emote.message, black, emote_back);
	    //std::cout << "Printing emote [" << emote.message << "] at " << emote_x << "," << emote_y << "; TTL: " << emote.ttl << "\n";
	    emote.deleted = false;
	}
    }
}
  
void gui_main_game_view::render(const screen_region viewport)
{
    const position_component * camera_pos = engine::globals::ecs->find_entity_component<position_component>(world::camera_handle);

    const int left_x = std::max ( 0, camera_pos->x - viewport.w/2 );
    const int top_y = std::max ( 0, camera_pos->y - viewport.h/2 );
    const int right_x = std::min ( landblock_width-1, camera_pos->x + viewport.w/2 );
    const int bottom_y = std::min ( landblock_height-2, camera_pos->y + viewport.h/2 -2 );

    const float sun_angle = world::sun_angle;
    
    int screen_y = viewport.y+2;
    for ( int y=top_y; y<=bottom_y; ++y ) {
        int screen_x = viewport.x;
        for ( int x=left_x; x<=right_x; ++x ) {
            const int region_idx = world::current_region->idx ( x,y );
            tile t = world::current_region->tiles[ region_idx ];
            if ( world::current_region->revealed[ region_idx ] ) {
                if ( world::current_region->visible[ region_idx ] ) {
                    auto finder = world::entity_render_list.find(region_idx);
                    if (finder == world::entity_render_list.end()) {
                        const color_t foreground = t.foreground;
                        const color_t background = t.background;
                        const float terrain_angle = t.surface_normal;
                        const float angle_difference = std::abs(terrain_angle - sun_angle);
                        float intensity_pct = 1.0 - (std::abs(angle_difference)/90.0F);
                        if (intensity_pct < 0.25) intensity_pct = 0.25;
                        if (intensity_pct > 1.0) intensity_pct = 1.0;
			if (world::current_region->tiles[region_idx].base_tile_type == tile_type::WATER) intensity_pct = 1.0;

                        const float red = std::get<0>(foreground) * intensity_pct;
                        const float green = std::get<1>(foreground) * intensity_pct;
                        const float blue = std::get<2>(foreground) * intensity_pct;

                        const color_t fg {
                            red,green,blue
                        };

                        //std::cout << "Ground angle: " << terrain_angle << ", difference: " << angle_difference << ", intensity: " << intensity_pct << "\n";

                        vterm::set_char_xy ( screen_x, screen_y, { t.display, fg, background } );
                    } else {
			vterm::set_char_xy ( screen_x, screen_y, finder->second );
                    }
                } else {
                    vterm::set_char_xy ( screen_x, screen_y, { t.display, color_t{16,16,64}, t.background } );
                }
            } else {
	      vterm::set_char_xy ( screen_x, screen_y, { '.', color_t{8,8,8}, t.background } );
            }
            ++screen_x;
        }
        ++screen_y;
    }
    
    render_heading(viewport, left_x, right_x, top_y, bottom_y);
}

}

#include "gui_main_game_view.h"
#include <iostream>
#include <sstream>
#include "../messages/mouse_motion_message.h"
#include "../game.h"

using std::stringstream;
using namespace engine;
using std::make_pair;

color_t gui_main_game_view::emote_background ( const chat_emote_color_t &col, const int& time_to_live )
{
    constexpr int multiplier = 8;
    const unsigned char full = multiplier * time_to_live;
  
    switch (col) {
      case chat_emote_color_t::YELLOW : return color_t{full, full, 0};
      case chat_emote_color_t::CYAN : return color_t{0, full, full};
      case chat_emote_color_t::GREEN : return color_t{0, full, 0};
      case chat_emote_color_t::MAGENTA : return color_t{full, 0, full};
      case chat_emote_color_t::RED : return color_t{full, 0, 0};
      case chat_emote_color_t::BLUE : return color_t{0, 0, full};
      case chat_emote_color_t::WHITE : return color_t{full, full, full};
    }
}

color_t gui_main_game_view::emote_foreground ( const chat_emote_color_t &col, const int& ttl )
{
    if (col == BLUE) return color_t{0,255,255};
    return color_t{0,0,0};
}
  
void gui_main_game_view::render_heading( const screen_region &viewport, const int &vp_left, const int &vp_right, const int &vp_top, const int &vp_bottom )
{
    // Heading
    constexpr char * heading{"<<< CORDEX OS 0.98 >>>"};
    constexpr color_t colors[] {
      color_t{ 0, 128, 128},
      color_t{ 0, 139, 139},
      color_t{ 0, 150, 150},
      color_t{ 0, 161, 161},
      color_t{ 0, 172, 172},
      color_t{ 0, 183, 183},
      color_t{ 0, 194, 194},
      color_t{ 0, 205, 205},
      color_t{ 0, 216, 216},
      color_t{ 0, 227, 227},
      color_t{ 0, 255, 255},
      color_t{ 0, 255, 255},
      color_t{ 0, 227, 227},
      color_t{ 0, 216, 216},
      color_t{ 0, 205, 205},
      color_t{ 0, 194, 194},
      color_t{ 0, 183, 183},
      color_t{ 0, 172, 172},
      color_t{ 0, 161, 161},
      color_t{ 0, 150, 150},
      color_t{ 0, 139, 139},
      color_t{ 0, 128, 128}
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
    
}

void gui_main_game_view::render_power_bar ( const screen_region& viewport, const int& vp_left, const int& vp_right, const int& vp_top, const int& vp_bottom )
{
    stringstream ss;
    ss << "<Power: " << world::stored_power << " / " << world::max_power << ">";
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
}

void gui_main_game_view::render_emotes ( const screen_region& viewport, const int& vp_left, const int& vp_right, const int& vp_top, const int& vp_bottom )
{
    vector<chat_emote_message> * emote_ptr = game_engine->messaging->get_messages_by_type<chat_emote_message>();
    for (chat_emote_message &emote : *emote_ptr) {
	if (emote.x > vp_left and emote.x < vp_right and emote.y > vp_top and emote.y < vp_bottom) {
	    const int size = emote.message.size();
	    int emote_x = emote.x - vp_left;
	    const int emote_y = emote.y - vp_top+2;
	    if (emote.x + size > vp_right) emote.x -= (vp_right - size);
	    
	    const color_t emote_back = emote_background(emote.color, emote.ttl);
	    const color_t emote_fore = emote_foreground(emote.color, emote.ttl);
	    
	    vterm::set_char_xy(emote_x, emote_y, {27, emote_back, emote_fore});
	    vterm::print(emote_x+1, emote_y, emote.message + " ", emote_fore, emote_back);
	    emote.deleted = false;
	}
    }
}


void gui_main_game_view::process_mouse_events()
{
    vector<mouse_motion_message> * mouse_movement = game_engine->messaging->get_messages_by_type<mouse_motion_message>();
    for (mouse_motion_message &m : *mouse_movement) {
	mouse_x = m.x;
	mouse_y = m.y;
	const int new_mouse_vx = mouse_x / 16;
	const int new_mouse_vy = mouse_y / 16;
	
	if (mouse_vx != new_mouse_vx or mouse_vy != new_mouse_vy) {	
	    mouse_vx = new_mouse_vx;
	    mouse_vy = new_mouse_vy;
	    mouse_hover_time = 0;
	} else {
	    ++mouse_hover_time;
	}
	m.deleted = true;
    }
    
    vector<command_message> * actions = game_engine->messaging->get_messages_by_type<command_message>();
    for (command_message &m : *actions) {
      if (m.command == RIGHT_CLICK) {
	  m.deleted = true;
	  if (mouse_vy > 3) {
	      world::paused = true;
	      // Change to options mode
	  }
      }
    }
}

string pad_to(const string &str, const int &num, const char paddingChar = ' ')
{
    string result = str;
    if(num > str.size())
    	result.insert(0, num - str.size(), paddingChar);
    return result;
}

unsigned char gender_icon(const short &gender) {
  switch (gender) {
    case gender_t::ASEXUAL : return 205;
    case gender_t::FEMALE : return 12;
    case gender_t::HERMAPHRODITE : return 236;
    case gender_t::MALE : return 11;
  }
  return ' ';
}

void gui_main_game_view::render_tooltip ( const screen_region& viewport, const int &region_x, const int &region_y ) {
    const int idx = world::current_region->idx( region_x, region_y );
    if (world::current_region->revealed[idx] != true) return;
  
    bool go_left = true;
    if (mouse_vx > (viewport.w/2)) go_left = false;
    
    vector<pair<string,color_t>> lines;
    
    //lines.push_back(make_pair("Right-Click For Options", magenta));
    lines.push_back(make_pair(world::current_region->tiles[idx].get_description(), green));
    lines.push_back(make_pair(world::current_region->tiles[idx].get_climate(), cyan));
    
    vector<position_component> * positions = game_engine->ecs->find_components_by_type<position_component>();
    for (const position_component &pos : *positions) {
	if (pos.x == region_x and pos.y == region_y) {
	    const int entity_id = pos.entity_id;
	    
	    settler_ai_component * settler = game_engine->ecs->find_entity_component<settler_ai_component>(entity_id);
	    if (settler != nullptr) {
		game_species_component * species = game_engine->ecs->find_entity_component<game_species_component>(entity_id);
		stringstream ss;
		ss << gender_icon(species->gender) << " " << settler->first_name << " " << settler->last_name << " (" << settler->profession_tag << ")";
		lines.push_back(make_pair(ss.str(), yellow));
	    } else {	    
		debug_name_component * debug_name = game_engine->ecs->find_entity_component<debug_name_component>(entity_id);
		if (debug_name != nullptr) lines.push_back(make_pair(debug_name->debug_name, white));
	    }
	}
    }
    
    int tip_length = 0;
    for (const pair<string,color_t> &line : lines) if (tip_length < line.first.size()) tip_length = line.first.size();
    
    if (go_left) {
	vterm::set_char_xy(mouse_vx-1, mouse_vy, { 26, dark_green, black });
	// Draw box
	int render_y = mouse_vy - (lines.size()/2);
	vterm::draw_box(mouse_vx - (tip_length + 3), render_y-1, tip_length+1, lines.size()+1, dark_green, black);
	for (const pair<string,color_t> &line : lines) {
	    vterm::print( mouse_vx - (tip_length+2), render_y, pad_to(line.first, tip_length), line.second, black );
	    ++render_y;
	}
    } else {
	vterm::set_char_xy(mouse_vx+1, mouse_vy, { 27, dark_green, black });    
	int render_y = mouse_vy - (lines.size()/2);
	vterm::draw_box(mouse_vx+2, render_y-1, tip_length+1, lines.size()+1, dark_green, black);
	for (const pair<string,color_t> &line : lines) {
	    vterm::print( mouse_vx +3, render_y, pad_to(line.first, tip_length), line.second, black );
	    ++render_y;
	}	
    }
}

void gui_main_game_view::render(const screen_region viewport)
{
    process_mouse_events();
  
    const position_component * camera_pos = game_engine->ecs->find_entity_component<position_component>(world::camera_handle);

    const int left_x = std::max ( 0, camera_pos->x - viewport.w/2 );
    const int top_y = std::max ( 0, camera_pos->y - viewport.h/2 );
    const int right_x = std::min ( landblock_width-1, camera_pos->x + viewport.w/2 );
    const int bottom_y = std::min ( landblock_height-2, camera_pos->y + viewport.h/2 -2 );

    const float sun_angle = world::sun_angle;
    
    int selected_region_x = 0;
    int selected_region_y = 0;
    
    int screen_y = viewport.y+2;
    for ( int y=top_y; y<=bottom_y; ++y ) {
        int screen_x = viewport.x;
        for ( int x=left_x; x<=right_x; ++x ) {
	    if (screen_x == mouse_vx and screen_y == mouse_vy) {
		selected_region_x = x;
		selected_region_y = y;
	    }
	  
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
			//vterm::set_char_xy ( screen_x, screen_y, finder->second );
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
    render_power_bar(viewport, left_x, right_x, top_y, bottom_y);
    render_emotes(viewport, left_x, right_x, top_y, bottom_y);
    
    if ( current_mode == PLAYING and mouse_vy > 2 and mouse_hover_time > 10) render_tooltip(viewport, selected_region_x, selected_region_y);
}

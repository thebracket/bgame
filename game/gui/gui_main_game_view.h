#include "../../engine/gui/gui_element_base.h"
#include "../../game/world/world.h"
#include <string>
#include <iostream>
#include <algorithm>
#include "../../engine/ecs.h"
#include "../../game/components/position_component.h"
#include <cmath>
#include "../messages/chat_emote_message.h"

using std::string;
using engine::screen_region;
using engine::gui_element_base;

enum gui_mode_t { PLAYING };

class gui_main_game_view : public gui_element_base {
public:
     gui_main_game_view ( const string name ) {
          tag = name;
     };

     virtual void render ( const screen_region viewport );

private:
     void render_heading( const engine::screen_region& viewport, const int& vp_left, const int& vp_right, const int& vp_top, const int& vp_bottom );
     void render_power_bar( const engine::screen_region& viewport, const int& vp_left, const int& vp_right, const int& vp_top, const int& vp_bottom );
     void render_emotes( const engine::screen_region& viewport, const int& vp_left, const int& vp_right, const int& vp_top, const int& vp_bottom );
     void render_tooltip( const screen_region& viewport, const int& region_x, const int& region_y );
     color_t emote_background(const chat_emote_color_t &col, const int &ttl);
     color_t emote_foreground(const chat_emote_color_t &col, const int &ttl);
     
     void process_mouse_events();
     
     int mouse_x = 0;
     int mouse_y = 0;
     int mouse_vx = 0;
     int mouse_vy = 0;
     int mouse_hover_time = 0;
     gui_mode_t current_mode = PLAYING;
};

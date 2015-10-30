#include "gui_main_game_command_panel.h"

namespace engine {

gui_main_game_command_panel::gui_main_game_command_panel ( const string name ) {
    tag = name;
}

void gui_main_game_command_panel::render ( const screen_region viewport ) {
    int x = viewport.x+1;
    int y = viewport.y+3;

    vterm::print(x,y-2,"Cordex 0.9 Beta",white,black);
    vterm::print(x,y,world::display_day_month,red,black);
    vterm::print(x,y+1,world::display_time,white,black);
    if (world::display_season == "Winter") {
      vterm::print(x,y+2,world::display_season,cyan,black);
    } else if (world::display_season == "Spring") {
      vterm::print(x,y+2,world::display_season,yellow,black);
    } else if (world::display_season == "Summer") {
      vterm::print(x,y+2,world::display_season,orange,black);
    } else {
      vterm::print(x,y+2,world::display_season,brown,black);
    }
    
    if (world::paused) {
	vterm::print(x,y+3," ** PAUSED **",yellow,black);
    }

    stringstream ss;
    ss << "Power: " << world::stored_power;
    vterm::print(x,y+5,ss.str(),yellow,black);
    ss.clear();

    int max_width = viewport.w-1;
    const float power_percent = float(world::stored_power) / float(world::max_power);
    float ticks = max_width * power_percent;
    const vterm::screen_character display_good {
        219, green, black
    };
    const vterm::screen_character display_bad {
        250, dark_green, black
    };
    for (int i=0; i<max_width; i++) {
        if (i <= ticks) {
            vterm::set_char_xy(x+i, y+6, display_good);
        } else {
            vterm::set_char_xy(x+i, y+6, display_bad);
        }
    }
    
    vterm::print(x,y+10,"SPACE",yellow,black);
    if (world::paused) {
	vterm::print(x+6,y+10,"unpause",grey,black);
    } else {
	vterm::print(x+6,y+10,"pause",grey,black);
    }
}

}
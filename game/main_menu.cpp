#include "main_menu.h"
#include "play_mode.h"
#include "world_gen_mode.h"
#include <fstream>
#include "world/worldgen.h"

using std::make_unique;
using std::ifstream;

bool fexists(const char *filename)
{
  ifstream ifile(filename);
  if (ifile) return true;
  return false;
}

void main_menu::init()
{
    world_available = fexists("world/LB1.dat");
    unique_ptr<gui_frame> frame = make_unique<gui_frame>("Outer", FILL_REGION);
    frame->add_child(make_unique<gui_static_text>("Title", "Black Future", 1, 1, red, black, true));
    if (world_available) {
	frame->add_child(make_unique<gui_static_text>("PlayGame",      	"              Play the Game              ", 1, 4, black, white, true));
    } else {
	frame->add_child(make_unique<gui_static_text>("PlayGame",      	" You Must Create World to Play the Game  ", 1, 4, black, dark_grey, true));      
    }
    frame->add_child(make_unique<gui_static_text>("BuildWorld",    	"            Create the World             ", 1, 5, grey, black, true));
    frame->add_child(make_unique<gui_static_text>("Quit",          	"              Quit Program               ", 1, 6, grey, black, true));

    menu_interface.add_child(std::move(frame));
    if (world_available) {
      selected_item = 0;
    } else {
      selected_item = 1;
    }
}

void main_menu::done()
{

}

pair< return_mode, unique_ptr< base_mode > > main_menu::tick ( const double time_elapsed )
{
    if (command::is_key_down(command::UP)) --selected_item;
    if (command::is_key_down(command::DOWN)) ++selected_item;
    if (selected_item < 0) selected_item = 0;
    if (selected_item > 2) selected_item = 2;

    gui_static_text * play_game = static_cast<gui_static_text *>(menu_interface.find_child("PlayGame"));
    gui_static_text * build_world = static_cast<gui_static_text *>(menu_interface.find_child("BuildWorld"));
    gui_static_text * quit = static_cast<gui_static_text *>(menu_interface.find_child("Quit"));

    if (selected_item == 0) {
	if (world_available) {
	    play_game->change_color(black,white);
	} else {
	    play_game->change_color(dark_grey,white);
	}
    } else {
	if (world_available) {
	    play_game->change_color(white,black);
	} else {
	    play_game->change_color(dark_grey,black);
	}
    }
    if (selected_item == 1) {
        build_world->change_color(black,white);
    } else {
        build_world->change_color(white,black);
    }
    if (selected_item == 2) {
        quit->change_color(black,white);
    } else {
        quit->change_color(white,black);
    }

    // Draw it
    menu_interface.render();

    // Menu branching
    if (selected_item == 2 and command::is_key_down(command::ENTER)) {
        return make_pair ( POP, NO_PUSHED_MODE ); // Quit
    }
    if (selected_item == 0 and world_available and command::is_key_down(command::ENTER)) {
        return make_pair ( PUSH, make_unique<play_mode>() ); // Play the game
    }
    if (selected_item == 1 and command::is_key_down(command::ENTER)) {
	return make_pair ( PUSH, make_unique<world_gen_mode>() ); // Create the world!
	
	// TODO: Go to world-builder mode and do this properly
	//worldgen::build_world();
	//world_available = true;
	//play_game->update_text("             Play the Game               ");
    }
    return make_pair ( CONTINUE, NO_PUSHED_MODE );
}

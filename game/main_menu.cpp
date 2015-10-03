#include "main_menu.h"
#include "play_mode.h"

using std::make_unique;

void main_menu::init()
{
    unique_ptr<gui_frame> frame = make_unique<gui_frame>("Outer", FILL_REGION);
    frame->add_child(make_unique<gui_static_text>("Title", "Black Future", 1, 1, red, black, true));
    frame->add_child(make_unique<gui_static_text>("PlayGame",      "    Play the Game   ", 1, 4, black, white, true));
    frame->add_child(make_unique<gui_static_text>("SomethingElse", "  Do something else ", 1, 5, grey, black, true));
    frame->add_child(make_unique<gui_static_text>("Quit",          "     Quit Program   ", 1, 6, grey, black, true));

    menu_interface.add_child(std::move(frame));
    selected_item = 0;
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
    gui_static_text * placeholder = static_cast<gui_static_text *>(menu_interface.find_child("SomethingElse"));
    gui_static_text * quit = static_cast<gui_static_text *>(menu_interface.find_child("Quit"));

    if (selected_item == 0) {
        play_game->change_color(black,white);
    } else {
        play_game->change_color(white,black);
    }
    if (selected_item == 1) {
        placeholder->change_color(black,white);
    } else {
        placeholder->change_color(white,black);
    }
    if (selected_item == 2) {
        quit->change_color(black,white);
    } else {
        quit->change_color(white,black);
    }

    // Draw it
    menu_interface.render();

    // For now, we quit after 100 iterations
    if (selected_item == 2 && command::is_key_down(command::ENTER)) {
        return make_pair ( POP, NO_PUSHED_MODE );
    }
    if (selected_item == 0 && command::is_key_down(command::ENTER)) {
        return make_pair ( PUSH, make_unique<play_mode>() );
    }
    return make_pair ( CONTINUE, NO_PUSHED_MODE );
}

#include "play_mode.h"
#include "world/world.h"

using namespace engine;
using std::make_unique;
using world::player_x;
using world::player_y;

void play_mode::init()
{
     init_gui();
     quitting = false;
     world::log.write ( "Welcome to @B_YELLOW@Black Future" );
     world::log.write ( "Use the @B_WHITE@arrow keys@WHITE@ to move, or press @B_RED@Q@WHITE@ to quit." );
     int widx = world::world_idx(5,5);
     world::current_region = new land_block(widx);
     player_x = 128;
     player_y = 128;
     world::current_region->make_radius_visible(player_x, player_y, 8);
}

void play_mode::init_gui()
{
     std::unique_ptr<gui_element_base> console = make_unique<gui_frame> ( "ConsolePanel", TRI_CONSOLE );
     console->add_child ( make_unique<gui_console> ( "Console" ) );

     std::unique_ptr<gui_element_base> main_panel = make_unique<gui_frame> ( "MainPanel", TRI_MAIN );
     main_panel->add_child ( make_unique<gui_main_game_view> ( "GameView" ) );

     std::unique_ptr<gui_element_base> command_panel = make_unique<gui_frame> ( "CommandPanel", TRI_PANEL );
     command_panel->add_child ( make_unique<gui_main_game_command_panel> ( "GamePanel" ) );

     ui.add_child ( std::move ( main_panel ) );
     ui.add_child ( std::move ( console ) );
     ui.add_child ( std::move ( command_panel ) );
}


void play_mode::done()
{
    delete world::current_region;
}

pair< return_mode, unique_ptr< base_mode > > play_mode::tick ( const double time_elapsed )
{
     if ( command::is_key_down ( command::Q ) ) quitting = true;
     bool moved = false;
     
     if ( command::is_key_down ( command::UP ) ) {
       --player_y;
       moved = true;
     }
     if ( command::is_key_down ( command::DOWN ) ) {
       ++player_y;
       moved = true;
     }
     if ( command::is_key_down ( command::LEFT ) ) {
       --player_x;
       moved = true;
     }
     if ( command::is_key_down ( command::RIGHT ) ) {       
       ++player_x;
       moved = true;
     }
     
     // Clamping
     if ( player_x < 0 ) player_x = 0;
     if ( player_x > landblock_width ) player_x = landblock_width;
     if ( player_y < 0 ) player_y = 0;
     if ( player_y > landblock_height ) player_y = landblock_height;

     // Visibility
     if (moved) {
       world::current_region->reset_visibility();
       world::current_region->make_radius_visible(player_x, player_y, 8);
     }

     ui.render();

     if ( quitting ) {
	  return make_pair ( POP, NO_PUSHED_MODE );
     }
     return make_pair ( CONTINUE, NO_PUSHED_MODE );
}

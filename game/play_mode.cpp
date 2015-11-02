#include "play_mode.h"
#include "world/world.h"
#include "../engine/globals.h"
#include "systems/system_factory.h"
#include "../engine/gui/gui_frame.h"
#include "../engine/gui/gui_static_text.h"
#include "gui/gui_console.h"
#include "gui/gui_main_game_view.h"
#include "gui/gui_main_game_command_panel.h"

using namespace engine;
using std::make_unique;

void play_mode::init_systems()
{
     engine::globals::ecs->add_system ( make_camera_system() );
     engine::globals::ecs->add_system ( make_calendar_system() );
     engine::globals::ecs->add_system ( make_obstruction_system() );
     engine::globals::ecs->add_system ( make_flowmap_system() );
     engine::globals::ecs->add_system ( make_power_system() );
     engine::globals::ecs->add_system ( make_settler_ai_system() );
     engine::globals::ecs->add_system ( make_viewshed_system() );
     engine::globals::ecs->add_system ( make_renderable_system() );
}

void play_mode::init()
{
     engine::globals::ecs->init();
     engine::globals::ecs->load_game("world/saved_game.dat");
     init_systems();
     
     init_gui();
     quitting = false;
     world::log.write ( "Welcome to @B_YELLOW@Black Future" );
     world::log.write ( "Use the @B_WHITE@arrow keys@WHITE@ to move, or press @B_RED@Q@WHITE@ to quit." );
     int widx = world::world_idx(2,2);
     world::current_region = new land_block(widx);
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
    world::current_region->save();
    engine::globals::ecs->save_game("world/saved_game.dat");
    delete world::current_region;
    engine::globals::ecs->done();
}

pair< return_mode, unique_ptr< base_mode > > play_mode::tick ( const double time_elapsed )
{
     if ( command::is_key_down ( command::Q ) ) quitting = true;

     ui.render();

     if ( quitting ) {
	  return make_pair ( POP, NO_PUSHED_MODE );
     }
     return make_pair ( CONTINUE, NO_PUSHED_MODE );
}

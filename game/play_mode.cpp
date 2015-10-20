#include "play_mode.h"
#include "world/world.h"
#include "../engine/ecs/entity_factory.h"

using namespace engine;
using std::make_unique;

void play_mode::init()
{
     engine::ecs::init();
     engine::ecs::entity test = engine::ecs::make_test_entity(130,130);
     engine::ecs::entity test2 = engine::ecs::make_test_entity(126,126);
     engine::ecs::entity test3 = engine::ecs::make_test_entity(120,127);
     engine::ecs::entity cordex = engine::ecs::make_cordex(128,128,0L);
     engine::ecs::entity camera = engine::ecs::make_camera_entity();
     engine::ecs::add_entity(camera);
     engine::ecs::add_entity(cordex);
     engine::ecs::add_entity(test);
     engine::ecs::add_entity(test2);
     engine::ecs::add_entity(test3);
     init_gui();
     quitting = false;
     world::log.write ( "Welcome to @B_YELLOW@Black Future" );
     world::log.write ( "Use the @B_WHITE@arrow keys@WHITE@ to move, or press @B_RED@Q@WHITE@ to quit." );
     int widx = world::world_idx(16,16);
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
    delete world::current_region;
    engine::ecs::done();
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

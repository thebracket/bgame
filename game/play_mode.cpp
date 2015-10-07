#include "play_mode.h"
#include "world/world.h"

using namespace engine;
using std::make_unique;

void play_mode::init()
{
     init_gui();
     count = 0;
     world::log.write("Welcome to @B_YELLOW@Black Future");
     world::log.write("Bert's attempt at writing a rogue-like.");
}

void play_mode::init_gui()
{
     std::unique_ptr<gui_element_base> console = make_unique<gui_frame> ( "ConsolePanel", TRI_CONSOLE );
     console->add_child( make_unique<gui_console> ( "Console") );
  
     std::unique_ptr<gui_element_base> main_panel = make_unique<gui_frame> ( "MainPanel", TRI_MAIN );
     console->add_child( make_unique<gui_main_game_view> ("GameView") );
     
     std::unique_ptr<gui_element_base> command_panel = make_unique<gui_frame> ( "CommandPanel", TRI_PANEL );
     console->add_child( make_unique<gui_main_game_command_panel> ("GamePanel") );
     
     ui.add_child ( std::move(main_panel) );
     ui.add_child ( std::move(console) );
     ui.add_child ( std::move(command_panel) );
}


void play_mode::done()
{

}

pair< return_mode, unique_ptr< base_mode > > play_mode::tick ( const double time_elapsed )
{
     ui.render();

     ++count;
     if ( count < 100 ) {
          return make_pair ( CONTINUE, NO_PUSHED_MODE );
     }
     return make_pair ( POP, NO_PUSHED_MODE );
}

#include "loading_screen.h"
#include "main_menu.h"
#include <memory>
#include "raws/raws.h"
#include "../engine/gui/gui_frame.h"
#include "../engine/gui/gui_static_text.h"
#include "game.h"

using std::make_unique;
using namespace engine;

void loading_screen::init()
{
     game_engine->load_image_resource("loading_screen.png", "splashscreen");
     loaded_assets = false;
     displayed = false; // Next time we are here we exit
     count  = 0;
     sg.children.push_back( make_unique<engine::scene_background>( "splashscreen" ) );
}

void loading_screen::done()
{
     // Nothing to do here.
}

pair< return_mode, unique_ptr< base_mode > > loading_screen::tick ( const double time_elapsed )
{
     if (count == 1) {
	raws::read_raws();
	game_engine->load_image_resource("spritesheet.png", "spritesheet");
	game_engine->load_image_resource("button1.png", "big_button_1");
	game_engine->load_image_resource("button2.png", "big_button_2");
	game_engine->load_image_resource("header.png", "header");
	game_engine->load_image_resource("main_menu.png", "menuscreen");
	game_engine->load_image_resource("paused.png", "paused");
	game_engine->load_image_resource("emote_bubble.png", "emote_bubble");
	game_engine->load_font_resource("../assets/font/disco___.ttf", "disco36", 36);
	game_engine->load_font_resource("../assets/font/disco___.ttf", "disco14", 14);
	game_engine->load_font_resource("../assets/font/disco___.ttf", "disco12", 12);
	game_engine->load_font_resource("../assets/font/disco___.ttf", "disco10", 10);	
     }
     
     // Exit conditions
     ++count;
     if (count > 10) loaded_assets = true;

     // We've already displayed the splash screen and gone to the menu; therefore,
     // popping by this way means we want to quit.
     if ( displayed ) {
          return make_pair ( POP, NO_PUSHED_MODE );
     }

     // If we haven't loaded the assets, keep going
     if ( !loaded_assets ) {
          return make_pair ( CONTINUE, NO_PUSHED_MODE );
     } else {
          // Assets are loaded, so we can continue to the main menu
          displayed = true;
          return make_pair ( PUSH, make_unique<main_menu>() );
     }
}

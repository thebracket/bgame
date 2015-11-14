#include "play_mode.h"
#include "world/world.h"
#include "systems/system_factory.h"
#include "game.h"
#include "raws/raws.h"
#include "game_render.h"
#include <sstream>

using namespace engine;
using std::make_unique;

void play_mode::init_systems()
{
     game_engine->ecs->add_system ( make_input_system() );
     game_engine->ecs->add_system ( make_camera_system() );
     game_engine->ecs->add_system ( make_calendar_system() );
     game_engine->ecs->add_system ( make_obstruction_system() );
     game_engine->ecs->add_system ( make_flowmap_system() );
     game_engine->ecs->add_system ( make_power_system() );
     game_engine->ecs->add_system ( make_settler_ai_system() );
     game_engine->ecs->add_system ( make_viewshed_system() );
     game_engine->ecs->add_system ( make_renderable_system() );

     SDL_Rect all {0, 0, 1024, 48};
     sg.children.push_back ( make_unique<scene_blit> ( "header", all, all ) );
     sg.children.push_back ( make_unique<game_render>() );
}

void play_mode::init()
{
     game_engine->ecs->init();
     game_engine->ecs->load_game ( "world/saved_game.dat" );
     init_systems();

     quitting = false;
     world::log.write ( "Welcome to @B_YELLOW@Black Future" );
     world::log.write ( "Use the @B_WHITE@arrow keys@WHITE@ to move, or press @B_RED@Q@WHITE@ to quit." );
     int widx = world::world_idx ( 2,2 );
     world::current_region = new land_block ( widx );
}

void play_mode::done()
{
     world::current_region->save();
     game_engine->ecs->save_game ( "world/saved_game.dat" );
     delete world::current_region;
     game_engine->ecs->done();
}

pair< return_mode, unique_ptr< base_mode > > play_mode::tick ( const double time_elapsed )
{
     if ( command::is_key_down ( command::Q ) ) {
          quitting = true;
     }

     if ( quitting ) {
          return make_pair ( POP, NO_PUSHED_MODE );
     }
     return make_pair ( CONTINUE, NO_PUSHED_MODE );
}

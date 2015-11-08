#include "input_system.h"
#include "../../engine/engine.h"
#include "../game.h"

using namespace engine::command;

void input_system::tick ( const double& duration_ms )
{
     if ( is_key_down ( SPACE ) )  game_engine->messaging->add_message<command_message> ( command_message ( TOGGLE_PAUSE ) );
     if ( is_key_down ( UP ) ) game_engine->messaging->add_message<command_message> ( command_message ( CAMERA_UP ) );
     if ( is_key_down ( DOWN ) ) game_engine->messaging->add_message<command_message> ( command_message ( CAMERA_DOWN ) );
     if ( is_key_down ( LEFT ) ) game_engine->messaging->add_message<command_message> ( command_message ( CAMERA_LEFT ) );
     if ( is_key_down ( RIGHT ) ) game_engine->messaging->add_message<command_message> ( command_message ( CAMERA_RIGHT ) );
     if ( mouse_x != 0 and mouse_y != 0 ) game_engine->messaging->add_message<mouse_motion_message>( mouse_motion_message( mouse_x, mouse_y ) );
     if ( left_click ) game_engine->messaging->add_message<command_message>( command_message( LEFT_CLICK ) );
     if ( right_click ) game_engine->messaging->add_message<command_message>( command_message( RIGHT_CLICK ) );
}

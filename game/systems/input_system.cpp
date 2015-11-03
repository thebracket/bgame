#include "input_system.h"
#include "../../engine/engine.h"

using namespace engine::command;
using engine::globals::messages;

void input_system::tick ( const double& duration_ms )
{
     if ( is_key_down ( SPACE ) )  messages->add_message<command_message> ( command_message ( TOGGLE_PAUSE ) );
     if ( is_key_down ( UP ) ) messages->add_message<command_message> ( command_message ( CAMERA_UP ) );
     if ( is_key_down ( DOWN ) ) messages->add_message<command_message> ( command_message ( CAMERA_DOWN ) );
     if ( is_key_down ( LEFT ) ) messages->add_message<command_message> ( command_message ( CAMERA_LEFT ) );
     if ( is_key_down ( RIGHT ) ) messages->add_message<command_message> ( command_message ( CAMERA_RIGHT ) );

}

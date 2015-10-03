#include <iostream>
#include <memory>
#include "engine/engine.h"
#include "test_mode.h"
#include "game/loading_screen.h"

using std::cout;
using std::make_unique;

int main()
{
     engine::init(engine::NCURSES);
     engine::main_loop ( make_unique<loading_screen>() );
}


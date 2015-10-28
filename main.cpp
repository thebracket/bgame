#include <iostream>
#include <memory>
#include "engine/engine.h"
#include "test_mode.h"
#include "game/loading_screen.h"

using std::cout;
using std::make_unique;

int main()
{
     engine::bracket_engine<engine::sdl2_backend> engine;
     engine.init();
     engine.main_loop( make_unique<loading_screen>() );
}


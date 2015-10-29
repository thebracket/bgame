#include <iostream>
#include <memory>
#include "engine/engine.h"
#include "test_mode.h"
#include "game/loading_screen.h"
#include "config.h"
#include "engine/globals.h"

#include "game/components/components.h"

using std::cout;
using std::make_unique;

int main()
{
     GAME engine;
     GAME_ECS ecs(component_factory);
     engine::globals::ecs = &ecs;
     engine.init();
     engine.main_loop( make_unique<loading_screen>() );
}


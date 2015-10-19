#pragma once

#include "base_component.h"

namespace engine {
namespace ecs {

class settler_ai_component : public base_component {
public:
    settler_ai_component() {
        type = settler_ai;
	next_tick = 0;
    };
    
    int next_tick;
};

}
}

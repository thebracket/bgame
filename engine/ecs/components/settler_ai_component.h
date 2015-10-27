#pragma once

#include "base_component.h"

namespace engine {
namespace ecs {

class settler_ai_component : public base_component {
public:
    settler_ai_component();
    
    int next_tick;
    
    virtual void save(fstream &lbfile);
    virtual void load(fstream &lbfile);
};

}
}

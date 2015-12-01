#pragma once

#include "../world/world.h"
#include "../../engine/ecs.hpp"

using engine::base_system;

class cordex_ai_system : public base_system {
public:
    cordex_ai_system() { system_name = "Cordex AI System"; }
    virtual void tick(const double &duration_ms);
private:
    void handle_build_orders();
    void handle_tree_chop_orders();
};

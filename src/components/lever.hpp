#pragma once

#include <vector>

struct lever_t {
    bool active = false;
    std::vector<std::size_t> targets;    
};

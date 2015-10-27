#pragma once

#include "component_types.h"

namespace engine {
namespace ecs {

struct calendar_component {
public:
    int entity_id;
    int handle;
    component_type type = calendar;
    
    calendar_component() {}
    calendar_component(const long &t) : system_time(t) {}
      
    long system_time = 0;
    short year = 0;
    char month = 0;
    char day = 0;
    char hour = 0;
    char minute = 0;
    
    double duration_buffer = 0.0;
    
};

}
}

#pragma once

#include "base_component.h"

namespace engine {
namespace ecs {

class calendar_component : public base_component {
public:
    calendar_component();
    calendar_component(const long &t);
    
    long system_time;
    short year;
    char month;
    char day;
    char hour;
    char minute;
    
    double duration_buffer;
    
    virtual void save(fstream &lbfile);
    virtual void load(fstream &lbfile);
};

}
}

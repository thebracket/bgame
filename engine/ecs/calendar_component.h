#pragma once

#include "base_component.h"
#include <vector>

using std::vector;

namespace engine {
namespace ecs {

class calendar_component : public base_component {
public:
    calendar_component() {
        type = calendar;
	system_time = 0L;
	duration_buffer = 0.0;
    };
    calendar_component(const long &t) : system_time(t) {
	type=calendar;
	duration_buffer = 0.0;
    }
    
    long system_time;
    double duration_buffer;
};

}
}

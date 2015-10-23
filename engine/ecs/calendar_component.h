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
    
    virtual void save(fstream &lbfile) {
	base_component::save(lbfile);
	lbfile.write ( reinterpret_cast<const char *> ( &system_time ), sizeof ( system_time ) );
	lbfile.write ( reinterpret_cast<const char *> ( &duration_buffer ), sizeof ( duration_buffer ) );
    };
    
    virtual void load(fstream &lbfile) {
	base_component::load(lbfile);
	lbfile.read ( reinterpret_cast<char *> ( &system_time ), sizeof ( system_time ) );
	lbfile.read ( reinterpret_cast<char *> ( &duration_buffer ), sizeof ( duration_buffer ) );
    };
};

}
}

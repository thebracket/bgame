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
	year = 2525;
	month = 0;
	day = 0;
	hour = 0;
	minute = 0;
    };
    calendar_component(const long &t) : system_time(t) {
	type=calendar;
	duration_buffer = 0.0;
	year = 2525;
	month = 0;
	day = 0;
	hour = 0;
	minute = 0;
    }
    
    long system_time;
    short year;
    char month;
    char day;
    char hour;
    char minute;
    
    double duration_buffer;
    
    virtual void save(fstream &lbfile) {
	base_component::save(lbfile);
	lbfile.write ( reinterpret_cast<const char *> ( &system_time ), sizeof ( system_time ) );
	lbfile.write ( reinterpret_cast<const char *> ( &year ), sizeof ( year ) );
	lbfile.write ( reinterpret_cast<const char *> ( &month ), sizeof ( month ) );
	lbfile.write ( reinterpret_cast<const char *> ( &day ), sizeof ( day ) );
	lbfile.write ( reinterpret_cast<const char *> ( &hour ), sizeof ( hour ) );
	lbfile.write ( reinterpret_cast<const char *> ( &minute ), sizeof ( minute ) );
	lbfile.write ( reinterpret_cast<const char *> ( &duration_buffer ), sizeof ( duration_buffer ) );
    };
    
    virtual void load(fstream &lbfile) {
	base_component::load(lbfile);
	lbfile.read ( reinterpret_cast<char *> ( &system_time ), sizeof ( system_time ) );
	lbfile.read ( reinterpret_cast<char *> ( &year ), sizeof ( year ) );
	lbfile.read ( reinterpret_cast<char *> ( &month ), sizeof ( month ) );
	lbfile.read ( reinterpret_cast<char *> ( &day ), sizeof ( day ) );
	lbfile.read ( reinterpret_cast<char *> ( &hour ), sizeof ( hour ) );
	lbfile.read ( reinterpret_cast<char *> ( &minute ), sizeof ( minute ) );
	lbfile.read ( reinterpret_cast<char *> ( &duration_buffer ), sizeof ( duration_buffer ) );
    };
};

}
}

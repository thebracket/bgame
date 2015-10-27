#include "calendar_component.h"

namespace engine {
namespace ecs {

calendar_component::calendar_component() {
    type = calendar;
    system_time = 0L;
    duration_buffer = 0.0;
    year = 2525;
    month = 0;
    day = 0;
    hour = 0;
    minute = 0;
};

calendar_component::calendar_component(const long &t) : system_time(t) {
    type=calendar;
    duration_buffer = 0.0;
    year = 2525;
    month = 0;
    day = 0;
    hour = 0;
    minute = 0;
}


void calendar_component::save(fstream &lbfile) {
    base_component::save(lbfile);
    lbfile.write ( reinterpret_cast<const char *> ( &system_time ), sizeof ( system_time ) );
    lbfile.write ( reinterpret_cast<const char *> ( &year ), sizeof ( year ) );
    lbfile.write ( reinterpret_cast<const char *> ( &month ), sizeof ( month ) );
    lbfile.write ( reinterpret_cast<const char *> ( &day ), sizeof ( day ) );
    lbfile.write ( reinterpret_cast<const char *> ( &hour ), sizeof ( hour ) );
    lbfile.write ( reinterpret_cast<const char *> ( &minute ), sizeof ( minute ) );
    lbfile.write ( reinterpret_cast<const char *> ( &duration_buffer ), sizeof ( duration_buffer ) );
};

void calendar_component::load(fstream &lbfile) {
    base_component::load(lbfile);
    lbfile.read ( reinterpret_cast<char *> ( &system_time ), sizeof ( system_time ) );
    lbfile.read ( reinterpret_cast<char *> ( &year ), sizeof ( year ) );
    lbfile.read ( reinterpret_cast<char *> ( &month ), sizeof ( month ) );
    lbfile.read ( reinterpret_cast<char *> ( &day ), sizeof ( day ) );
    lbfile.read ( reinterpret_cast<char *> ( &hour ), sizeof ( hour ) );
    lbfile.read ( reinterpret_cast<char *> ( &minute ), sizeof ( minute ) );
    lbfile.read ( reinterpret_cast<char *> ( &duration_buffer ), sizeof ( duration_buffer ) );
};

}
}
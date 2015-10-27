#include "debug_name_component.h"

namespace engine {
namespace ecs {

debug_name_component::debug_name_component() {
    type = name;
};

debug_name_component::debug_name_component(const string &NAME) : debug_name(NAME) {
    type = name;
};

void debug_name_component::save(fstream &lbfile) {
    base_component::save(lbfile);
    int name_size = debug_name.size();
    lbfile.write ( reinterpret_cast<const char *> ( &name_size ), sizeof ( name_size ) );
    lbfile.write ( debug_name.c_str(), name_size );
};

void debug_name_component::load(fstream &lbfile) {
    base_component::load(lbfile);
    int name_size = 0;
    lbfile.read ( reinterpret_cast<char *> ( &name_size ), sizeof ( name_size ) );
    char buf[name_size+1];
    lbfile.read ( buf, name_size );
    buf[name_size] = '\0';
    debug_name = string(buf);
};

}
}
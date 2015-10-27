#include "viewshed_component.h"

namespace engine {
namespace ecs {

viewshed_component::viewshed_component() {
    type = viewshed;
}

viewshed_component::viewshed_component(const viewshed_type &T, const int &R) : scanner_type(T), scanner_range(R) {
    type = viewshed;
}

void viewshed_component::save(fstream &lbfile) {
    base_component::save(lbfile);
    lbfile.write ( reinterpret_cast<const char *> ( &scanner_type ), sizeof ( scanner_type ) );
    lbfile.write ( reinterpret_cast<const char *> ( &scanner_range ), sizeof ( scanner_range ) );
}

void viewshed_component::load(fstream &lbfile) {
    base_component::load(lbfile);
    lbfile.read ( reinterpret_cast<char *> ( &scanner_type ), sizeof ( scanner_type ) );
    lbfile.read ( reinterpret_cast<char *> ( &scanner_range ), sizeof ( scanner_range ) );
    last_visibility.clear();
}

}
}
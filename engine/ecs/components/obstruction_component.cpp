#include "obstruction_component.h"

namespace engine {
namespace ecs {

obstruction_component::obstruction_component() {
    type = obstruction;
    blocks_entry = true;
    blocks_visibility = true;
};

void obstruction_component::save(fstream &lbfile) {
    base_component::save(lbfile);
    lbfile.write ( reinterpret_cast<const char *> ( &blocks_entry ), sizeof ( blocks_entry ) );
    lbfile.write ( reinterpret_cast<const char *> ( &blocks_visibility ), sizeof ( blocks_visibility ) );
};

void obstruction_component::load(fstream &lbfile) {
    base_component::load(lbfile);
    lbfile.read ( reinterpret_cast<char *> ( &blocks_entry ), sizeof ( blocks_entry ) );
    lbfile.read ( reinterpret_cast<char *> ( &blocks_visibility ), sizeof ( blocks_visibility ) );
};

}
}
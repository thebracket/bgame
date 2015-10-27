#include "settler_ai_component.h"

namespace engine {
namespace ecs {

settler_ai_component::settler_ai_component() {
    type = settler_ai;
    next_tick = 0;
}

void settler_ai_component::save(fstream &lbfile) {
    base_component::save(lbfile);
    lbfile.write ( reinterpret_cast<const char *> ( &next_tick ), sizeof ( next_tick ) );
}

void settler_ai_component::load(fstream &lbfile) {
    base_component::load(lbfile);
    lbfile.read ( reinterpret_cast<char *> ( &next_tick ), sizeof ( next_tick ) );
}

}
}
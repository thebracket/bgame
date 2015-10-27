#include "position_component.h"

namespace engine {
namespace ecs {

position_component::position_component() {
    type = position;
    moved=false;
};
position_component::position_component(const int &X, const int &Y) : x(X), y(Y) {
    type=position;
    moved=false;
};

void position_component::save(fstream &lbfile) {
    base_component::save(lbfile);
    lbfile.write ( reinterpret_cast<const char *> ( &x ), sizeof ( x ) );
    lbfile.write ( reinterpret_cast<const char *> ( &y ), sizeof ( y ) );
};

void position_component::load(fstream &lbfile) {
    base_component::load(lbfile);
    lbfile.read ( reinterpret_cast<char *> ( &x ), sizeof ( x ) );
    lbfile.read ( reinterpret_cast<char *> ( &y ), sizeof ( y ) );
};
}
}
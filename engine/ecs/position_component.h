#pragma once

#include "base_component.h"

namespace engine {
namespace ecs {
 
class position_component : public base_component {
public:
    position_component() { type = position; moved=false; };
    position_component(const int &X, const int &Y) : x(X), y(Y) { type=position; moved=false; };
    int x;
    int y;
    bool moved;
    
     virtual void save(fstream &lbfile) {
	base_component::save(lbfile);
	lbfile.write ( reinterpret_cast<const char *> ( &x ), sizeof ( x ) );
	lbfile.write ( reinterpret_cast<const char *> ( &y ), sizeof ( y ) );
    };
    
    virtual void load(fstream &lbfile) {
	base_component::load(lbfile);
	lbfile.read ( reinterpret_cast<char *> ( &x ), sizeof ( x ) );
	lbfile.read ( reinterpret_cast<char *> ( &y ), sizeof ( y ) );
    };
};

}
}

#pragma once

#include "base_component.h"

namespace engine {
namespace ecs {

class settler_ai_component : public base_component {
public:
    settler_ai_component() {
        type = settler_ai;
	next_tick = 0;
    };
    
    int next_tick;
    
    virtual void save(fstream &lbfile) {
	base_component::save(lbfile);
	lbfile.write ( reinterpret_cast<const char *> ( &next_tick ), sizeof ( next_tick ) );
    };
    
    virtual void load(fstream &lbfile) {
	base_component::load(lbfile);
	lbfile.read ( reinterpret_cast<char *> ( &next_tick ), sizeof ( next_tick ) );
    };
};

}
}

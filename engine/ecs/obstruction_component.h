#pragma once

#include "base_component.h"

namespace engine {
namespace ecs {

class obstruction_component : public base_component {
public:
    obstruction_component() {
        type = obstruction;
        blocks_entry = true;
        blocks_visibility = true;
    };

    bool blocks_entry;
    bool blocks_visibility;

    virtual void save(fstream &lbfile) {
        base_component::save(lbfile);
        lbfile.write ( reinterpret_cast<const char *> ( &blocks_entry ), sizeof ( blocks_entry ) );
        lbfile.write ( reinterpret_cast<const char *> ( &blocks_visibility ), sizeof ( blocks_visibility ) );
    };

    virtual void load(fstream &lbfile) {
        base_component::load(lbfile);
        lbfile.read ( reinterpret_cast<char *> ( &blocks_entry ), sizeof ( blocks_entry ) );
        lbfile.read ( reinterpret_cast<char *> ( &blocks_visibility ), sizeof ( blocks_visibility ) );
    };
};

}
}
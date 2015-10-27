#pragma once

#include "base_component.h"

namespace engine {
namespace ecs {

class obstruction_component : public base_component {
public:
    obstruction_component();

    bool blocks_entry;
    bool blocks_visibility;

    virtual void save(fstream &lbfile);
    virtual void load(fstream &lbfile);
};

}
}
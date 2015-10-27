#pragma once

#include "base_component.h"

namespace engine {
namespace ecs {

class position_component : public base_component {
public:
    position_component();
    position_component(const int &X, const int &Y);

    int x;
    int y;
    bool moved;

    virtual void save(fstream &lbfile);
    virtual void load(fstream &lbfile);
};

}
}

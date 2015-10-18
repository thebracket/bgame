#pragma once

#include "base_component.h"

namespace engine {
namespace ecs {
 
struct position_component : public base_component {
    position_component() : type(position) {};
    position_component(const int &X, const int &Y) : x(X), y(Y), type(position) {};
    int x;
    int y;
};

}
}

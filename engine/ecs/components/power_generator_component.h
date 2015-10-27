#pragma once

#include "base_component.h"
#include "../../raws/raw_power_generator.h"

using engine::raws::power_generator_condition;

namespace engine {
namespace ecs {

class power_generator_component : public base_component {
public:
    power_generator_component();
    power_generator_component(const int &new_amount, const power_generator_condition &mode);

    int amount;
    power_generator_condition generator_mode;

    virtual void save(fstream &lbfile);
    virtual void load(fstream &lbfile);
};

}
}
#pragma once

#include "base_component.h"
#include "../raws/raw_power_generator.h"

using engine::raws::power_generator_condition;

namespace engine {
namespace ecs {

class power_generator_component : public base_component {
public:
    power_generator_component() {
        type = power_generator;
    };
    power_generator_component(const int &new_amount, const power_generator_condition &mode) :
        amount(new_amount), generator_mode(mode) {
        type=power_generator;
    };

    int amount;
    power_generator_condition generator_mode;

    virtual void save(fstream &lbfile) {
        base_component::save(lbfile);
        lbfile.write ( reinterpret_cast<const char *> ( &amount ), sizeof ( amount ) );
        lbfile.write ( reinterpret_cast<const char *> ( &generator_mode ), sizeof ( generator_mode ) );
    };

    virtual void load(fstream &lbfile) {
        base_component::load(lbfile);
        lbfile.read ( reinterpret_cast<char *> ( &amount ), sizeof ( amount ) );
        lbfile.read ( reinterpret_cast<char *> ( &generator_mode ), sizeof ( generator_mode ) );
    };
};

}
}
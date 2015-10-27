#include "power_generator_component.h"

namespace engine {
namespace ecs {
 
    power_generator_component::power_generator_component() {
        type = power_generator;
    }
    
    power_generator_component::power_generator_component(const int &new_amount, const power_generator_condition &mode) :
        amount(new_amount), generator_mode(mode) {
        type=power_generator;
    }
  
    void power_generator_component::save(fstream &lbfile) {
        base_component::save(lbfile);
        lbfile.write ( reinterpret_cast<const char *> ( &amount ), sizeof ( amount ) );
        lbfile.write ( reinterpret_cast<const char *> ( &generator_mode ), sizeof ( generator_mode ) );
    }
    
    void power_generator_component::load(fstream &lbfile) {
        base_component::load(lbfile);
        lbfile.read ( reinterpret_cast<char *> ( &amount ), sizeof ( amount ) );
        lbfile.read ( reinterpret_cast<char *> ( &generator_mode ), sizeof ( generator_mode ) );
    }
}
}
#include "base_component.h"

namespace engine {
namespace ecs {
 
void base_component::load ( fstream& lbfile )
{
    // Note: not reading component_type, it will be read in the factory
    lbfile.read ( reinterpret_cast<char *> ( &entity_id ), sizeof ( entity_id ) );
}

void base_component::save ( fstream& lbfile )
{
    lbfile.write ( reinterpret_cast<const char *> ( &type ), sizeof ( type ) );
    lbfile.write ( reinterpret_cast<const char *> ( &entity_id ), sizeof ( entity_id ) );
}
  
}
}
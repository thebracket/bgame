#include "entity.h"
#include <iostream>

namespace engine {
namespace ecs {

void entity::load ( fstream& lbfile )
{
    lbfile.read ( reinterpret_cast<char *> ( &handle ), sizeof ( handle ) );
}

void entity::save ( fstream& lbfile )
{
    lbfile.write ( reinterpret_cast<const char *> ( &handle ), sizeof ( handle ) );
}

entity construct_entity_from_file ( fstream& lbfile )
{
    entity e;
    e.load(lbfile);
    return e;
}

}
}
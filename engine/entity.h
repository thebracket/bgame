#pragma once

#include <fstream>

using std::fstream;

namespace engine {

class entity {
public:
    int handle;
    
    void load ( fstream& lbfile )
    {
	lbfile.read ( reinterpret_cast<char *> ( &handle ), sizeof ( handle ) );
    }

    void save ( fstream& lbfile )
    {
	lbfile.write ( reinterpret_cast<const char *> ( &handle ), sizeof ( handle ) );
    }

  int component_count = 0;
};

inline entity construct_entity_from_file ( fstream& lbfile )
{
    entity e;
    e.load(lbfile);
    return e;
}

}

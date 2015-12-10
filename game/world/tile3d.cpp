#include "tile3d.hpp"
#include "../components/component_types.h"

using namespace serialization_generic;

void tile_t::save ( std::fstream& lbfile )
{
    save_primitive<bool> ( lbfile, solid );
    save_primitive<unsigned char>( lbfile, ground );
    save_primitive<unsigned char>( lbfile, climate );
    save_primitive<unsigned char>( lbfile, covering );
}

void tile_t::load ( std::fstream& lbfile )
{
    load_primitive<bool>( lbfile, solid );
    load_primitive<unsigned char>( lbfile, ground );
    load_primitive<unsigned char>( lbfile, climate );
    save_primitive<unsigned char>( lbfile, covering );
}

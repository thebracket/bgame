#include "tile3d.hpp"
#include "../components/component_types.h"
#include <iostream>

using namespace serialization_generic;

void tile_t::save ( Poco::DeflatingOutputStream& lbfile )
{
    save_primitive<bool> ( lbfile, solid );
    save_primitive<uint8_t>( lbfile, water_level );
    save_primitive<unsigned char>( lbfile, base_tile_type );
    save_primitive<unsigned char>( lbfile, ground );
    save_primitive<unsigned char>( lbfile, climate );
    save_primitive<unsigned char>( lbfile, covering );
}

void tile_t::load ( Poco::InflatingInputStream& lbfile )
{
    load_primitive<bool>( lbfile, solid );
    load_primitive<uint8_t>( lbfile, water_level );
    load_primitive<unsigned char>( lbfile, base_tile_type );
    load_primitive<unsigned char>( lbfile, ground );
    load_primitive<unsigned char>( lbfile, climate );
    load_primitive<unsigned char>( lbfile, covering );
}

#include "solar_system_body.hpp"

void solar_system_body_t::load ( std::fstream& lbfile )
{
    lbfile.read ( reinterpret_cast<char *> ( &system_idx ), sizeof ( system_idx ) );
    lbfile.read ( reinterpret_cast<char *> ( &body_type ), sizeof ( body_type ) );
}

void solar_system_body_t::save ( std::fstream& lbfile ) const
{
    lbfile.write ( reinterpret_cast<const char *> ( &system_idx ), sizeof( system_idx ) );
    lbfile.write ( reinterpret_cast<const char *> ( &body_type ), sizeof( body_type ) );
}

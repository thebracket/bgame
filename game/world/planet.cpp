#include "planet.hpp"

region_t* planet_t::get_region ( const uint8_t region_index )
{
    if ( regions[ region_index ].get() == nullptr ) {
      load_region( region_index );
    }
    return regions[ region_index ].get();
}

tile_t* planet_t::get_tile ( const location_t location )
{
    region_t * region = get_region( location.region );
    return region->get_tile( location.x, location.y, location.z );
}

void planet_t::load_region ( const uint8_t region_index )
{

}

void planet_t::save_region ( const uint8_t region_index )
{

}

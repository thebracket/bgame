#include "planet.hpp"
#include <sstream>
#include <fstream>
#include <Poco/InflatingStream.h>
#include <Poco/DeflatingStream.h>
#include <iostream>

using std::stringstream;
using std::fstream;

namespace world {
  
std::unique_ptr < planet_t > planet = std::make_unique < planet_t > ();
  
}

region_t* planet_t::get_region ( const uint8_t region_index )
{
    if ( regions[ region_index ].get() == nullptr ) {
      load_region( region_index );
    }
    return regions[ region_index ].get();
}

region_t* planet_t::create_region ( const uint8_t region_index )
{
    regions [ region_index ] = std::make_unique < region_t > ();
    return regions [ region_index ].get();
}

void planet_t::free_region ( const uint8_t region_index )
{
    regions [ region_index ].reset();
}

tile_t* planet_t::get_tile ( const location_t location )
{
    region_t * region = get_region( location.region );
    return region->get_tile( location.x, location.y, location.z );
}

std::string planet_t::get_filename ( const uint8_t region_index ) const
{
    stringstream ss;
    ss << "world/REGION_" << +region_index << ".dat";
    return ss.str();
}

void planet_t::load_region ( const uint8_t region_index )
{
    std::cout << "Loading region " << +region_index << "\n";
    const std::string filename = get_filename( region_index );
    region_t * region = create_region( region_index );
    fstream lbfile(filename, std::ios::in | std::ios::binary);
    Poco::InflatingInputStream inflate(lbfile, Poco::InflatingStreamBuf::STREAM_GZIP);
    region->load ( inflate );
}

void planet_t::save_region ( const uint8_t region_index )
{
    const std::string filename = get_filename( region_index );
    region_t * region = get_region( region_index );
    fstream lbfile(filename, std::ios::out | std::ios::binary);
    Poco::DeflatingOutputStream deflate(lbfile, Poco::DeflatingStreamBuf::STREAM_GZIP);
    region->save( deflate );
    deflate.close();
}

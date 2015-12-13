#include "region.hpp"

void region_t::save ( Poco::DeflatingOutputStream& lbfile )
{
     // TODO: Save revealed status
     for ( tile_t &tile : tiles ) {
          tile.save ( lbfile );
     }
}

void region_t::load ( Poco::InflatingInputStream& lbfile )
{
     // TODO: Load revealed status
     tiles.clear();
     for ( int i=0; i<REGION_TILES; ++i ) {
          tile_t tile;
          tile.load ( lbfile );
          tiles.push_back ( tile );
     }
}

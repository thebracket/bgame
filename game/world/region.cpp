#include "region.hpp"
#include "../components/component_loader.h"

void region_t::save ( Poco::DeflatingOutputStream& lbfile )
{
     for (const bool &r : revealed) {
	  save_primitive<bool>( lbfile, r );
     }
     for ( tile_t &tile : tiles ) {
          tile.save ( lbfile );
     }
}

void region_t::load ( Poco::InflatingInputStream& lbfile )
{
     revealed.clear();
     for (auto i=0; i<REGION_TILES; ++i) {
	  bool reveal;
	  load_primitive<bool>( lbfile, reveal );
	  revealed.push_back( reveal );
     }
     tiles.clear();
     for ( int i=0; i<REGION_TILES; ++i ) {
          tile_t tile;
          tile.load ( lbfile );
          tiles.push_back ( tile );
     }
}

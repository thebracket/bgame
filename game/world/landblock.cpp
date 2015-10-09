#include "landblock.h"
#include "../../engine/rng.h"
#include "geometry.h"

using geometry::project_angle;
using geometry::line_func;
using geometry::DEGRAD;

land_block::land_block()
{
  tiles.resize(tiles_per_landblock);
  for (tile &t : tiles) {
    int symbol = engine::roll_dice(1,6);
    switch (symbol) {
      case 1 : t.display = '.'; break;
      case 2 : t.display = '#'; break;
      case 3 : t.display = '`'; break;
      case 4 : t.display = '\''; break;
      case 5 : t.display = '"'; break;
      case 6 : t.display = '%'; break;
    }
  }
}

void land_block::make_radius_visible ( const int x, const int y, const int radius )
{
     tiles[idx ( x,y )].visible = true;
     tiles[idx ( x,y )].revealed = true;
     for ( int angle=0; angle<360; angle+=1 ) {
          const double radians = angle * DEGRAD;
          pair<int,int> destination = project_angle ( x,y,radius,angle );
          line_func ( x,y,destination.first,destination.second,[this] ( int tx, int ty ) {
               if ( tx >= 0 and tx <= landblock_width and ty >=0 and ty <= landblock_height ) {
                    tiles[idx ( tx,ty )].visible = true;
                    tiles[idx ( tx,ty )].revealed = true;
               }
          } );
     }
}

void land_block::reset_visibility()
{
  for (tile &t : tiles)
    t.visible = false;
}

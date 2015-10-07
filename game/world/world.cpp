#include "world.h"
#include <utility>
#include <functional>
#include <cstdlib>
#include <cmath>

using std::pair;

namespace world {

constexpr double DEGRAD = 3.14159/180;

inline pair<int,int> project_angle ( const int x, const int y, const double radius, const int degrees_radians )
{
     return std::make_pair ( x + radius*std::cos ( degrees_radians ), y + radius*std::sin ( degrees_radians ) );
}

/*
 * Perform a function for each line element between x1/y1 and x2/y2. Uses Bresenham's
 * algorithm; initial implementation from
 * http://www.roguebasin.com/index.php?title=Bresenham%27s_Line_Algorithm
 */
void line_func ( int x1, int y1, const int x2, const int y2, std::function<void ( int,int ) > func )
{
     int delta_x ( x2-x1 );
     signed char const ix ( ( delta_x > 0 ) - ( delta_x < 0 ) );
     delta_x = std::abs ( delta_x ) << 1;

     int delta_y ( y2 - y1 );
     signed char const iy ( ( delta_y > 0 ) - ( delta_y < 0 ) );
     delta_y = std::abs ( delta_y ) << 1;

     func ( x1,y1 );

     if ( delta_x >= delta_y ) {
          // error may go below zero
          int error ( delta_y - ( delta_x >> 1 ) );

          while ( x1 != x2 ) {
               if ( ( error >= 0 ) && ( error || ( ix > 0 ) ) ) {
                    error -= delta_x;
                    y1 += iy;
               }
               // else do nothing

               error += delta_y;
               x1 += ix;

               func ( x1, y1 );
          }
     } else {
          // error may go below zero
          int error ( delta_x - ( delta_y >> 1 ) );

          while ( y1 != y2 ) {
               if ( ( error >= 0 ) && ( error || ( iy > 0 ) ) ) {
                    error -= delta_y;
                    x1 += ix;
               }
               // else do nothing

               error += delta_x;
               y1 += iy;

               func ( x1, y1 );
          }
     }
}

void make_radius_visible ( const int x, const int y, const int radius )
{
     current_region.tiles[current_region.idx ( x,y )].visible = true;
     current_region.tiles[current_region.idx ( x,y )].revealed = true;
     for ( int angle=0; angle<360; angle+=1 ) {
          const double radians = angle * DEGRAD;
          pair<int,int> destination = project_angle ( x,y,radius,angle );
          line_func ( x,y,destination.first,destination.second,[] ( int tx, int ty ) {
               if ( tx >= 0 and tx <= landblock_width and ty >=0 and ty <= landblock_height ) {
                    current_region.tiles[current_region.idx ( tx,ty )].visible = true;
                    current_region.tiles[current_region.idx ( tx,ty )].revealed = true;
               }
          } );
     }
}

void reset_visibility()
{
  for (tile &t : current_region.tiles)
    t.visible = false;
}


int player_x = 128;
int player_y = 128;

console log;
land_block current_region;

}

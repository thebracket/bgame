#include "geometry.hpp"
#include <cstdlib>
#include <cmath>

namespace geometry {

/*
 * From a given point x/y, project forward radius units (generally tiles) at an angle of degrees_radians degrees
 * (in radians).
 */
pair<int,int> project_angle ( const int x, const int y, const double radius, const double degrees_radians )
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


}
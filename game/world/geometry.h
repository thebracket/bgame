#pragma once

#include <functional>
#include <utility>

using std::pair;

namespace geometry {

constexpr double DEGRAD = 3.14159/180;

/*
 * From a given point x/y, project forward radius units (generally tiles) at an angle of degrees_radians degrees
 * (in radians).
 */
pair<int,int> project_angle ( const int x, const int y, const double radius, const int degrees_radians );

/*
 * Perform a function for each line element between x1/y1 and x2/y2. Uses Bresenham's
 * algorithm; initial implementation from
 * http://www.roguebasin.com/index.php?title=Bresenham%27s_Line_Algorithm
 */
void line_func ( int x1, int y1, const int x2, const int y2, std::function<void ( int,int ) > func );

}

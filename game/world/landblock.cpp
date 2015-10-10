#include "landblock.h"
#include "geometry.h"
#include <sstream>
#include <string>
#include <fstream>

using geometry::project_angle;
using geometry::line_func;
using geometry::DEGRAD;
using std::stringstream;
using std::string;
using std::fstream;

land_block::land_block()
{
     tiles.resize ( tiles_per_landblock );
}

void land_block::make_radius_visible ( const int x, const int y, const int radius )
{
     constexpr double sweep_degrees_radians = 1 * DEGRAD;
     constexpr double three_sixy_degrees_radians = 360 * DEGRAD;

     tiles[idx ( x,y )].visible = true;
     tiles[idx ( x,y )].revealed = true;
     for ( int angle=0; angle<three_sixy_degrees_radians; angle+=sweep_degrees_radians ) {
          pair<int,int> destination = project_angle ( x, y, radius, angle );
          line_func ( x,y,destination.first,destination.second, [this] ( int tx, int ty ) {
               if ( tx >= 0 and tx <= landblock_width and ty >=0 and ty <= landblock_height ) {
                    tiles[idx ( tx,ty )].visible = true;
                    tiles[idx ( tx,ty )].revealed = true;
               }
          } );
     }
}

void land_block::reset_visibility()
{
     for ( tile &t : tiles )
          t.visible = false;
}

string land_block::get_filename() const
{
    stringstream filename_stream;
    filename_stream << "world/LB" << index << ".dat";
    return filename_stream.str();
}

void land_block::save()
{
    const string filename = get_filename();
    fstream lbfile(filename, std::ios::out | std::ios::binary);
    for (tile &t : tiles) {
	t.save(lbfile);
    }
    lbfile.close();
}

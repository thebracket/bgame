#include "landblock.h"
#include <sstream>
#include <string>
#include <fstream>

using std::stringstream;
using std::string;
using std::fstream;

land_block::land_block()
{
     tiles.resize ( tiles_per_landblock );
}

land_block::land_block(int index_to_load)
{
    index = index_to_load;
    tiles.resize ( tiles_per_landblock );
    load();
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

void land_block::load()
{
    const string filename = get_filename();
    fstream lbfile(filename, std::ios::in | std::ios::binary);
    for (tile &t : tiles) {
	t.load(lbfile);
    }
    lbfile.close();
}

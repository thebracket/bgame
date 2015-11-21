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
     visible.resize( tiles_per_landblock );
     revealed.resize ( tiles_per_landblock );
     std::fill(revealed.begin(), revealed.end(), false);
}

land_block::land_block(int index_to_load)
{
    index = index_to_load;
    tiles.resize ( tiles_per_landblock );
    visible.resize( tiles_per_landblock );
    revealed.resize ( tiles_per_landblock );
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
    for (unsigned int i=0; i<revealed.size(); ++i) {
	char state = 0;
	if (revealed[i]) state = 1;
	lbfile.write(&state,1);
    }
    for (tile &t : tiles) {      
	t.save(lbfile);
    }
    lbfile.close();
}

void land_block::load()
{
    const string filename = get_filename();
    fstream lbfile(filename, std::ios::in | std::ios::binary);
    for (unsigned int i=0; i<revealed.size(); ++i) {
      char state = 0;
      lbfile.read(&state,1);
      if (state == 1) {
	revealed[i] = true;
      } else {
	revealed[i] = false;
      }
    }
    for (tile &t : tiles) {
	t.load(lbfile);
    }
    lbfile.close();
}



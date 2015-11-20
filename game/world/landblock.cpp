#include "landblock.h"
#include <sstream>
#include <string>
#include <fstream>
#include <tuple>
#include <unordered_set>
#include <map>
#include <iostream>

using std::stringstream;
using std::string;
using std::fstream;
using std::unordered_set;

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

void land_block::spread_flow( vector<std::tuple<short,int,int>> &new_cells, unordered_set<int> &visited, const int &x, const int &y, const short &distance, unordered_map<int, bool> &walkable ) {
    const int index = idx (x,y);
    if (revealed[index]==false) return;
    if (visited.find( index ) != visited.end()) return;
    if (walkable.find( index ) != walkable.end()) return;
    if (x < 0) return;
    if (x > landblock_width-1) return;
    if (y < 0) return;
    if (y > landblock_height-1) return;
    
    visited.insert( index );
    new_cells.push_back ( std::make_tuple( distance, x, y ) );
}

std::shared_ptr< navigation_path > land_block::find_path ( pair< int, int > start, pair< int, int > end, unordered_map<int, bool> &walkable )
{
    std::cout << "PATH REQUEST FROM [" << start.first << "," << start.second << "] to [" << end.first << "," << end.second << "]\n";
    // For now, we're just using a really inefficient brute-force approach just to get going; this will be optimized later!
    
    // If we start at the end, then return null - nothign to do!
    if (start.first == end.first and start.second == end.second) return nullptr;
    
    vector<short> flow_map;
    flow_map.resize( landblock_height * landblock_width );
    std::fill ( flow_map.begin(), flow_map.end(), 32000);
    unordered_set<int> visited;
    vector<std::tuple<short,int,int>> active_cells; // (distance, x, y)
    vector<std::tuple<short,int,int>> new_cells;
    
    bool found_it = false;
    active_cells.push_back( std::make_tuple( 0, end.first, end.second ) );
    while ( active_cells.size() > 0 ) {
	for (std::tuple<short,int,int> &cell : active_cells) {
	    const short distance = std::get<0>(cell);
	    const int x = std::get<1>(cell);
	    const int y = std::get<2>(cell);
	    if ( x == start.first and y == start.second ) { found_it = true; }
	    flow_map[ idx(x, y) ] = distance;
	    
	    spread_flow( new_cells, visited, x-1, y, distance+1, walkable );
	    spread_flow( new_cells, visited, x+1, y, distance+1, walkable );
	    spread_flow( new_cells, visited, x, y-1, distance+1, walkable );
	    spread_flow( new_cells, visited, x, y+1, distance+1, walkable );
	}
	std::swap(active_cells, new_cells);
	new_cells.clear();
    }
  
    if (!found_it) {
	std::cout << "Destination not found!\n";
	return nullptr;
    }
    
    std::shared_ptr<navigation_path> result = std::make_shared<navigation_path>();
    result->destination = end;
    
    std::pair<int,int> current = start;
    int n = 0;
    char last_direction = -1;
    while (current.first != end.first or current.second != end.second) {
	std::cout << "Step " << n << "... at [" << current.first << "," << current.second << "]... ";
	std::map<short,char> options;
	if (last_direction!= 2) options[ flow_map[ idx ( current.first-1, current.second ) ] ] = 1;
	if (last_direction!= 1) options[ flow_map[ idx ( current.first+1, current.second ) ] ] = 2;
	if (last_direction!= 4) options[ flow_map[ idx ( current.first, current.second-1 ) ] ] = 3;
	if (last_direction!= 3) options[ flow_map[ idx ( current.first, current.second+1 ) ] ] = 4;
	
	auto it = options.begin();
	int next_x = current.first;
	int next_y = current.second;
	switch (it->second) {
	  case 1: --next_x ;break;
	  case 2: ++next_x ;break;
	  case 3: --next_y ;break;
	  case 4: ++next_y ;break;
	}
	last_direction = it->second;
	std::cout << "to [" << next_x << "," << next_y << "]\n";
	result->steps.push ( std::make_pair( next_x, next_y ));
	current.first = next_x;
	current.second = next_y;
	++n;
	std::cout << "Steps created: " << n << "\n";
    }
    
    return result;
}


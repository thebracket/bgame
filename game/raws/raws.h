#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <vector>
#include <utility>
#include <tuple>

using std::string;
using std::vector;
using std::pair;
using std::tuple;
using std::make_tuple;

namespace raws
{

/*
 * Load the database defining the game, from the "../raws/" folder. Raw files to
 * read are listed in "index".
 */
void read_raws();

/*
 * Create an entity and related components using definitions in the raw files.
 * Returns the entity handle.
 */
int create_structure_from_raws ( const string& name, const int& x, const int& y );
int create_placeholder_structure_from_raws ( const string& name, const int& x, const int& y );
int create_item_from_raws ( const string &name );

string get_random_starting_profession();
string get_random_first_name ( const short &gender );
string get_random_last_name();

SDL_Rect get_tile_source ( const int idx );
SDL_Rect get_tile_source_by_name ( const string &name );

vector< tuple<string,int,vector<string>>> get_buildables();
vector<string> get_buildable_requirements ( const string &name );

}

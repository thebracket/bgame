#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <vector>
#include <utility>
#include <tuple>
#include "base_raw.h"
#include "../world/location.hpp"

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
int create_structure_from_raws(const string& name, const location_t loc);
int create_placeholder_structure_from_raws(const string& name,
		const location_t loc);
int create_item_from_raws(const string &name);

pair<string, raws::base_raw*> get_random_starting_profession();
string get_random_first_name(const short &gender);
string get_random_last_name();

SDL_Rect get_tile_source(const int idx);
SDL_Rect get_tile_source_by_name(const string &name);

vector<tuple<string, int, vector<string>>> get_buildables();
vector<string> get_buildable_requirements(const string &name);
vector<base_raw *> get_possible_reactions_for_structure(
		const string &structure_name);

int get_tile_idx_by_name(const string &name);

}

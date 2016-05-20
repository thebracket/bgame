#include "raws.hpp"
#include "lua_bridge.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <memory>

std::unique_ptr<lua_lifecycle> lua_handle;

string_table_t first_names_male;
string_table_t first_names_female;
string_table_t last_names;

enum read_mode_t {NONE, TILE_MATERIAL};
read_mode_t read_mode = NONE;

std::vector<std::string> split ( const std::string str, const char delimiter )
{
     std::vector<std::string> internal;
     std::stringstream ss ( str ); // Turn the string into a stream.
     std::string tok;

     while ( getline ( ss, tok, delimiter ) ) {
          internal.push_back ( tok );
     }

     return internal;
}

void load_string_table(const std::string filename, string_table_t &target) {
	std::ifstream f(filename);
	std::string line;
	while (getline(f, line))
	{
		target.strings.push_back(line);
	}
}

void load_raws() {
	// Load string tables for first names and last names
	load_string_table("world_defs/first_names_male.txt", first_names_male);
	load_string_table("world_defs/first_names_female.txt", first_names_female);
	load_string_table("world_defs/last_names.txt", last_names);

	// Setup LUA
	lua_handle = std::make_unique<lua_lifecycle>();

	// Load game data via LUA
	string_table_t raw_index;
	load_string_table("world_defs/index.txt", raw_index);
	for (const std::string &filename : raw_index.strings) {
		load_lua_script("world_defs/" + filename);
	}

	// Extract game tables
}

#include "raws.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

string_table_t first_names_male;
string_table_t first_names_female;
string_table_t last_names;

std::unordered_map<std::string, rltk::color_t> color_table;
std::unordered_map<std::string, uint8_t> glyph_table;
std::unordered_map<int, raw::tile_material> tile_material_table;

enum read_mode_t {NONE, TILE_MATERIAL};
read_mode_t read_mode = NONE;
raw::tile_material new_tile_material;

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

uint8_t find_glpyh(const std::string &name) {
	auto finder = glyph_table.find(name);
	if (finder == glyph_table.end()) {
		std::cout << "WARNING: No such glyph - " << name << "\n";
		return 0;
	} else {
		return finder->second;
	}
}

rltk::color_t find_color(const std::string &name) {
	auto finder = color_table.find(name);
	if (finder == color_table.end()) {
		std::cout << "WARNING: No such color - " << name << "\n";
		return rltk::colors::BLACK;
	} else {
		return finder->second;
	}
}

bool start_multi_line(std::vector<std::string> chunks) {
	if (chunks[0] == "TILE_MATERIAL") {
		read_mode = TILE_MATERIAL;
		new_tile_material = raw::tile_material();
		new_tile_material.name = chunks[1];
		return true;
	}
	return false;
}

bool parse_multi_line(std::vector<std::string> chunks) {
	if (read_mode == TILE_MATERIAL) {
		if (chunks[0] == "INDEX") {
			new_tile_material.index = std::stoi(chunks[1]);
			return true;
		} else if (chunks[0] == "GLYPH") {
			new_tile_material.glyph = find_glpyh(chunks[1]);
			return true;
		} else if (chunks[0] == "COLOR") {
			new_tile_material.foreground = find_color(chunks[1]);
			new_tile_material.background = find_color(chunks[2]);
			return true;
		}
	}
	return false;
}

void finish_multi_line_tag() {
	switch (read_mode) {
		TILE_MATERIAL : tile_material_table[new_tile_material.index] = new_tile_material; break;
		NONE : std::cout << "WARNING: Committing even though there is no read mode!\n"; break;
	}

	read_mode = NONE;
}

void parse_raw_file(const string_table_t &raw) {
	bool in_multi_line = false;
	for (const std::string &line : raw.strings) {
		if (!line.empty() && !(line[0]=='#')) {
			auto chunks = split(line, ':');
			bool done = false;

			if (in_multi_line) {
				done = parse_multi_line(chunks);
			} else {
				if (chunks[0] == "GLYPH") {
					glyph_table[chunks[1]] = static_cast<uint8_t>(std::stoi(chunks[2]));
					done = true;
				} else if (chunks[0] == "COLOR") {
					color_table[chunks[1]] = rltk::color_t{ static_cast<uint8_t>(std::stoi(chunks[2])), static_cast<uint8_t>(std::stoi(chunks[3])), static_cast<uint8_t>(std::stoi(chunks[4])) };
					done = true;
				} else if (chunks[0] == "TILE_MATERIAL") {
					in_multi_line = true;
					done = start_multi_line(chunks);
				}
			}

			if (!done) std::cout << "WARNING: Don't know how to parse a " << chunks[0] << "\n";
		} else {
			if (line.empty() && in_multi_line) {
				finish_multi_line_tag();
				in_multi_line = false;
			}
		}
	}
}

void load_raws() {
	// Load string tables for first names and last names
	load_string_table("world_defs/first_names_male.txt", first_names_male);
	load_string_table("world_defs/first_names_female.txt", first_names_female);
	load_string_table("world_defs/last_names.txt", last_names);

	// Load game data
	string_table_t raw_index;
	load_string_table("world_defs/index.txt", raw_index);
	for (const std::string &filename : raw_index.strings) {
		string_table_t raw_data;
		load_string_table("world_defs/" + filename, raw_data);
		parse_raw_file(raw_data);
	}
}

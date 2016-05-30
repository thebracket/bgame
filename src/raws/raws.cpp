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

std::unordered_map<uint8_t, tile_type_t> tile_types;
std::unordered_map<std::string, uint8_t> tile_type_index;
std::unordered_map<uint16_t, tile_content_t> tile_contents;
std::unordered_map<std::string, uint16_t> tile_contents_index;

std::unordered_map<std::string, clothing_t> clothing_types;
std::vector<profession_t> starting_professions;

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

/*
void print_lua_table(lua_State *L, std::string table_name = "", int depth = 0)
{
	if (table_name != "") {
		std::cout << table_name << "\n";
		for (int i=0; i<table_name.size(); ++i) std::cout << "-";
		std::cout << "\n";
		lua_getglobal(L, table_name.c_str());
	    lua_pushnil(L);
	}

    while(lua_next(L, -2) != 0)
    {
    	std::string key = lua_tostring(L, -2);

    	if (depth == 0) std::cout << "\n";
    	for (int i=0; i<depth*3; ++i) std::cout << " ";
    	std::cout << "[" << key << "]";
    	if (lua_istable(L, -1)) {
    		std::cout << " is a table\n";
    		lua_pushstring(L, key.c_str());
    		lua_gettable(L, -2);
    		print_lua_table(L, "", depth+1);
    	} else {
    		std::cout << " is a " << luaL_typename(L, -1) << "\n";
    	}
    	//printf("%s - %s\n", lua_tostring(L, -2), luaL_typename(L, -1));
    	lua_pop(L, 1);
    }
}
*/

rltk::color_t read_lua_color(std::string field) {
	rltk::color_t col;
	lua_pushstring(lua_state, field.c_str());
	lua_gettable(lua_state, -2);
	while (lua_next(lua_state, -2) != 0) {
		std::string subfield = lua_tostring(lua_state, -2);
		if (subfield == "r") col.r = lua_tonumber(lua_state, -1);
		if (subfield == "g") col.g = lua_tonumber(lua_state, -1);
		if (subfield == "b") col.b = lua_tonumber(lua_state, -1);
		lua_pop(lua_state, 1);
	}
	return col;
}

void read_tile_types() {
	lua_getglobal(lua_state, "tile_types");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
    	tile_type_t tt;

    	std::string key = lua_tostring(lua_state, -2);
    	tt.name = key;
    	
    	lua_pushstring(lua_state, key.c_str());
    	lua_gettable(lua_state, -2);
    	while (lua_next(lua_state, -2) != 0) {
    		std::string field = lua_tostring(lua_state, -2);
    		if (field == "index") tt.index = lua_tonumber(lua_state, -1);
    		if (field == "glyph") tt.glyph = lua_tonumber(lua_state, -1);
    		if (field == "background") tt.bg = read_lua_color("background");
    		if (field == "foreground") tt.fg = read_lua_color("foreground");
            if (field == "name") tt.nice_name = lua_tostring(lua_state, -1);
    		lua_pop(lua_state, 1);
    	}

    	//std::cout << key << " is " << tt.index << ": " << +tt.glyph << ", " << +tt.bg.r << "/" << +tt.bg.g << "/" << +tt.bg.b << " \n";
    	tile_types[tt.index] = tt;
    	tile_type_index[tt.name] = tt.index;

    	lua_pop(lua_state, 1);
    }
}

void read_tile_contents() {
	lua_getglobal(lua_state, "tile_content");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
    	tile_content_t tt;

    	std::string key = lua_tostring(lua_state, -2);
    	tt.name = key;
    	
    	lua_pushstring(lua_state, key.c_str());
    	lua_gettable(lua_state, -2);
    	while (lua_next(lua_state, -2) != 0) {
    		std::string field = lua_tostring(lua_state, -2);
    		if (field == "index") tt.index = lua_tonumber(lua_state, -1);
    		if (field == "glyph") tt.glyph = lua_tonumber(lua_state, -1);
    		if (field == "background") tt.bg = read_lua_color("background");
    		if (field == "foreground") tt.fg = read_lua_color("foreground");
            if (field == "stairs") {
                std::string stairs_type = lua_tostring(lua_state, -1);
                if (stairs_type == "updown") tt.stairs = 1;
                if (stairs_type == "up") tt.stairs = 2;
                if (stairs_type == "down") tt.stairs = 3;
            }
            if (field == "name") tt.nice_name = lua_tostring(lua_state, -1);
    		lua_pop(lua_state, 1);
    	}

    	//std::cout << key << " is " << tt.index << ": " << +tt.glyph << ", " << +tt.bg.r << "/" << +tt.bg.g << "/" << +tt.bg.b << " \n";
    	tile_contents[tt.index] = tt;
    	tile_contents_index[tt.name] = tt.index;

    	lua_pop(lua_state, 1);
    }
}

void read_clothing() {
    lua_getglobal(lua_state, "clothing");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        clothing_t c;

        std::string key = lua_tostring(lua_state, -2);

        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while (lua_next(lua_state, -2) != 0) {
            std::string field = lua_tostring(lua_state, -2);

            if (field == "name") c.name = lua_tostring(lua_state, -1);
            if (field == "slot") c.slot = lua_tostring(lua_state, -1);
            if (field == "description") c.description = lua_tostring(lua_state, -1);
            if (field == "colors") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string color = lua_tostring(lua_state, -1);
                    c.colors.push_back(color);
                    lua_pop(lua_state, 1);
                }
            }

            lua_pop(lua_state, 1);
        }
        clothing_types[key] = c;

        lua_pop(lua_state, 1);
    }
}

void read_professions() {
    lua_getglobal(lua_state, "starting_professions");
    lua_pushnil(lua_state);

    while(lua_next(lua_state, -2) != 0)
    {
        std::string key = lua_tostring(lua_state, -2);
        profession_t p;

        lua_pushstring(lua_state, key.c_str());
        lua_gettable(lua_state, -2);
        while (lua_next(lua_state, -2) != 0) {
            std::string field = lua_tostring(lua_state, -2);
            if (field == "name") p.name = lua_tostring(lua_state, -1);
            // Stat mods
            if (field == "modifiers") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    std::string stat = lua_tostring(lua_state, -2);
                    int modifier = lua_tonumber(lua_state, -1);
                    if (stat == "str") p.strength = modifier;
                    if (stat == "dex") p.dexterity = modifier;
                    if (stat == "con") p.constitution = modifier;
                    if (stat == "int") p.intelligence = modifier;
                    if (stat == "wis") p.wisdom = modifier;
                    if (stat == "cha") p.charisma = modifier;
                    if (stat == "com") p.comeliness = modifier;
                    if (stat == "eth") p.ethics = modifier;
                    lua_pop(lua_state, 1);
                }
            }

            // Starting clothes
            if (field == "clothing") {
                lua_pushstring(lua_state, field.c_str());
                lua_gettable(lua_state, -2);
                while (lua_next(lua_state, -2) != 0) {
                    const std::string gender_specifier = lua_tostring(lua_state, -2);
                    lua_pushstring(lua_state, gender_specifier.c_str());
                    lua_gettable(lua_state, -2);
                    while (lua_next(lua_state, -2) != 0) {
                        const std::string slot = lua_tostring(lua_state, -2);
                        const std::string item = lua_tostring(lua_state, -1);
                        int gender_tag = 0;
                        if (gender_specifier == "male") gender_tag = 1;
                        if (gender_specifier == "female") gender_tag = 2;
                        p.starting_clothes.push_back( std::make_tuple(gender_tag, slot, item));
                        lua_pop(lua_state, 1);
                    }
                    lua_pop(lua_state, 1);
                }
            }

            lua_pop(lua_state, 1);
        }
        starting_professions.push_back(p);
        lua_pop(lua_state, 1);
    }
}

void load_game_tables() {
	read_tile_types();
	read_tile_contents();
    read_clothing();
    read_professions();
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
	load_game_tables();
}

uint8_t get_tile_type_index(const std::string name) {
	auto finder = tile_type_index.find(name);
	if (finder != tile_type_index.end()) {
		return finder->second;
	} else {
		throw std::runtime_error("Unknown tile type: " + name);
	}
}

uint16_t get_tile_contents_index(const std::string name) {
    //std::cout << "Tile type: " << name;
	auto finder = tile_contents_index.find(name);
	if (finder != tile_contents_index.end()) {
        //std::cout << " = " << finder->second << "\n";
		return finder->second;
	} else {
		throw std::runtime_error("Unknown tile contents: " + name);
	}
}

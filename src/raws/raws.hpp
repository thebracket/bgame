#pragma once

#include <vector>
#include <string>
#include <boost/container/flat_map.hpp>
#include <tuple>
#include <rltk.hpp>
#include <bitset>

struct string_table_t {
	std::vector<std::string> strings;

	inline std::string random_entry(rltk::random_number_generator rng=rltk::random_number_generator()) {
		const int position = rng.roll_dice(1, strings.size()) - 1;
		return strings[position];
	}
};

struct tile_type_t {
	std::string name = "";
	std::string nice_name = "";
	int index;
	uint8_t glyph;
	rltk::color_t fg;
	rltk::color_t bg;
};

struct tile_content_t {
	std::string name = "";
	std::string nice_name = "";
	int index;
	uint8_t glyph;
	rltk::color_t fg;
	rltk::color_t bg;
	uint8_t stairs = 0;
};

struct clothing_t {
	std::string name= "";
	std::vector<std::string> colors;
	std::string slot;
	std::string description;
};

struct profession_t {
	std::string name = "";	
	short strength = 0;
	short dexterity = 0;
	short constitution = 0;
	short intelligence = 0;
	short wisdom = 0;
	short charisma = 0;
	short comeliness = 0;
	short ethics;
	std::vector< std::tuple< uint8_t, std::string, std::string >> starting_clothes;
};

constexpr int NUMBER_OF_ITEM_CATEGORIES = 4;
constexpr int COMPONENT = 0;
constexpr int TOOL_CHOPPING = 1;
constexpr int TOOL_DIGGING = 2;
constexpr int WEAPON = 3;

struct item_def_t {
	std::string tag = "";
	std::string name = "";
	std::string description = "";
	std::bitset<NUMBER_OF_ITEM_CATEGORIES> categories;
	uint8_t glyph;
	rltk::color_t fg;
	rltk::color_t bg;
};

enum provides_t { provides_sleep, provides_food };

struct building_provides_t {
	provides_t provides;
	int energy_cost;
};

struct building_def_t {
	std::string tag = "";
	std::string name = "";
	std::vector<std::string> components;
	std::pair<std::string, int> skill;
	std::vector<building_provides_t> provides;	
	int width = 1;
	int height = 1;
	std::vector<rltk::vchar> glyphs;
};

struct reaction_t {
	std::string tag = "";
	std::string name = "";
	std::string workshop = "";
	std::vector<std::pair<std::string, int>> inputs;
	std::vector<std::pair<std::string, int>> outputs;
	std::string skill;
	int difficulty;
	bool automatic = false;
};

extern string_table_t first_names_male;
extern string_table_t first_names_female;
extern string_table_t last_names;

extern boost::container::flat_map<uint8_t, tile_type_t> tile_types;
extern boost::container::flat_map<std::string, uint8_t> tile_type_index;
extern boost::container::flat_map<uint16_t, tile_content_t> tile_contents;
extern boost::container::flat_map<std::string, uint16_t> tile_contents_index;

extern boost::container::flat_map<std::string, clothing_t> clothing_types;
extern std::vector<profession_t> starting_professions;

extern boost::container::flat_map<std::string, item_def_t> item_defs;
extern boost::container::flat_map<std::string, building_def_t> building_defs;
extern boost::container::flat_map<std::string, reaction_t> reaction_defs;
extern boost::container::flat_map<std::string, std::vector<std::string>> reaction_building_defs;

void load_raws();
uint8_t get_tile_type_index(const std::string name);
uint16_t get_tile_contents_index(const std::string name);

void spawn_item_on_ground(const int x, const int y, const int z, const std::string &tag);
void spawn_item_in_container(const std::size_t container_id, const std::string &tag);

std::string to_proper_noun_case(const std::string &original);
std::vector<std::string> split ( const std::string str, const char delimiter );

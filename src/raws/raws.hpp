#pragma once

#include <vector>
#include <string>
#include <boost/container/flat_map.hpp>
#include <boost/container/flat_set.hpp>
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

constexpr int NUMBER_OF_ITEM_CATEGORIES = 5;
constexpr int COMPONENT = 0;
constexpr int TOOL_CHOPPING = 1;
constexpr int TOOL_DIGGING = 2;
constexpr int WEAPON_MELEE = 3;
constexpr int WEAPON_RANGED = 4;

struct item_def_t {
	std::string tag = "";
	std::string name = "";
	std::string description = "";
	std::bitset<NUMBER_OF_ITEM_CATEGORIES> categories;
	uint8_t glyph;
	rltk::color_t fg;
	rltk::color_t bg;
	int damage_n=0, damage_d=0, damage_mod=0;
	int range = 0;
	std::string ammo;
	int stack_size = 1;
};

enum provides_t { provides_sleep, provides_food, provides_seating, provides_desk, provides_door, 
		provides_wall, provides_floor, provides_stairs_up, provides_stairs_down, provides_stairs_updown,
		provides_ramp, provides_light };

struct building_provides_t {
	provides_t provides;
	int energy_cost = 0;
	int radius = 0;
	rltk::color_t color = rltk::colors::WHITE;
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
	int power_drain = 0;
};

struct biome_type_t {
	std::string name = "";
	int8_t min_rain = 0;
	int8_t max_rain = 100;
	int8_t min_temp = -100;
	int8_t max_temp = 100;
	uint8_t min_mutation = 0;
	uint8_t max_mutation = 100;
	uint8_t soil_pct = 50;
	uint8_t sand_pct = 50;
	std::vector<uint8_t> occurs;
	uint8_t worldgen_glyph;
	rltk::color_t worldgen_color;
	std::vector<std::pair<std::string,int>> plants;
	std::vector<std::string> wildlife;
	int deciduous_tree_chance = 0;
	int evergreen_tree_chance = 0;
};

enum material_def_spawn_type_t { cluster_rock, rock, soil, sand };

struct material_def_t {
	std::string tag = "";
	std::string name = "";
	material_def_spawn_type_t spawn_type;
	std::string parent_material_tag = "";
	uint8_t glyph;
	rltk::color_t fg;
	rltk::color_t bg;
	uint8_t hit_points;
	std::string mines_to_tag = "";
	std::string mines_to_tag_second = "";
	std::string layer = "";
};

struct plant_t {
	std::string tag = "";
	std::string name = "";
	uint8_t glyph;
	rltk::color_t fg;
	rltk::color_t bg;
	std::string provides = "";
};

enum diet_t { diet_omnivore, diet_herbivore, diet_carnivore };
enum alignment_t { align_good, align_neutral, align_evil };

struct raw_species_t {
	std::string tag = "";
	std::string name = "";
	std::string male_name = "";
	std::string female_name = "";
	std::string collective_name = "";
	std::string description = "";
	boost::container::flat_set<std::pair<std::string,int>> stat_mods;
	std::vector<std::tuple<std::string, int, int>> body_parts;
	diet_t diet;
	alignment_t alignment;
	int max_age;
	int infant_age;
	int child_age;
	uint8_t glyph;
};

struct creature_attack_t {
	std::string type;
	int hit_bonus;
	int damage_n_dice;
	int damage_dice;
	int damage_mod;
};

enum creature_ai_t { creature_grazer };

struct raw_creature_t {
	std::string tag = "";
	std::string name = "";
	std::string male_name = "";
	std::string female_name = "";
	std::string collective_name = "";
	std::string description = "";
	boost::container::flat_set<std::pair<std::string,int>> stats;
	std::vector<std::tuple<std::string, int, int>> body_parts;
	int armor_class;
	std::vector<creature_attack_t> attacks;
	int yield_hide;
	int yield_meat;
	creature_ai_t ai;
	uint8_t glyph;
	rltk::color_t fg;
	int hp_n, hp_dice, hp_mod;
	int group_size_n_dice, group_size_dice, group_size_mod;
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

extern std::vector<biome_type_t> biome_defs;
extern boost::container::flat_map<std::string, std::size_t> material_defs_idx;
extern std::vector<material_def_t> material_defs;
extern boost::container::flat_map<std::string, std::size_t> plant_defs_idx;
extern std::vector<plant_t> plant_defs;

extern boost::container::flat_map<std::string, raw_species_t> species_defs;
extern boost::container::flat_map<std::string, raw_creature_t> creature_defs;

void load_raws();
uint8_t get_tile_type_index(const std::string name);
uint16_t get_tile_contents_index(const std::string name);

void spawn_item_on_ground(const int x, const int y, const int z, const std::string &tag, const std::size_t &material);
void spawn_item_in_container(const std::size_t container_id, const std::string &tag, const std::size_t &material);

inline std::size_t get_material_by_tag(const std::string &tag) {
	auto finder = material_defs_idx.find(tag);
	return finder->second;
}
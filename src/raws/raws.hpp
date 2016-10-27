#pragma once

#include <vector>
#include <string>
#include <boost/container/flat_map.hpp>
#include <boost/container/flat_set.hpp>
#include <tuple>
#include <rltk.hpp>
#include <bitset>
#include "../components/item_carried.hpp"
#include "reaction_input.hpp"

struct string_table_t {
	std::vector<std::string> strings;

	inline std::string random_entry(rltk::random_number_generator rng=rltk::random_number_generator()) {
		const int position = rng.roll_dice(1, static_cast<int>(strings.size())) - 1;
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
	std::string slot = "";
	std::string description = "";
	float armor_class = 0.0F;
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
	short ethics = 0;
	std::vector< std::tuple< uint8_t, std::string, std::string >> starting_clothes;
};

constexpr int NUMBER_OF_ITEM_CATEGORIES = 6;
constexpr int COMPONENT = 0;
constexpr int TOOL_CHOPPING = 1;
constexpr int TOOL_DIGGING = 2;
constexpr int WEAPON_MELEE = 3;
constexpr int WEAPON_RANGED = 4;
constexpr int WEAPON_AMMO = 5;

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
	int initiative_penalty = 0;
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
	std::vector<reaction_input_t> components;
	std::pair<std::string, int> skill;
	std::vector<building_provides_t> provides;	
	int width = 1;
	int height = 1;
	std::vector<rltk::vchar> glyphs;
	bool emits_smoke = false;
};


struct reaction_t {
	std::string tag = "";
	std::string name = "";
	std::string workshop = "";
	std::vector<reaction_input_t> inputs;
	std::vector<std::pair<std::string, int>> outputs;
	std::string skill = "";
	int difficulty = 10;
	bool automatic = false;
	int power_drain = 0;
	bool emits_smoke = false;
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

struct material_def_t {
	std::string tag = "";
	std::string name = "";
	material_def_spawn_type_t spawn_type = rock;
	std::string parent_material_tag = "";
	uint8_t glyph;
	rltk::color_t fg;
	rltk::color_t bg;
	uint8_t hit_points = 0;
	std::string mines_to_tag = "";
	std::string mines_to_tag_second = "";
	std::string layer = "";
	std::vector<std::string> ore_materials;
	int damage_bonus = 0;
	float ac_bonus = 0.0F;
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
	boost::container::flat_map<std::string,int> stat_mods;
	std::vector<std::tuple<std::string, int, int>> body_parts;
	diet_t diet = diet_omnivore;
	alignment_t alignment = align_neutral;
	int max_age = 90;
	int infant_age = 5;
	int child_age = 12;
	uint8_t glyph = '@';
};

struct creature_attack_t {
	std::string type = "";
	int hit_bonus = 0;
	int damage_n_dice = 1;
	int damage_dice = 6;
	int damage_mod = 0;
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
	int armor_class=10;
	std::vector<creature_attack_t> attacks;
	int yield_hide=0;
	int yield_meat=0;
	int yield_bone=0;
	int yield_skull=0;
	creature_ai_t ai;
	uint8_t glyph;
	rltk::color_t fg;
	int hp_n, hp_dice, hp_mod;
	int group_size_n_dice, group_size_dice, group_size_mod;
};

struct native_population_t {
	std::string name = "";
	short aggression = 0;
	short strength = 0;
	short dexterity = 0;
	short constitution = 0;
	short intelligence = 0;
	short wisdom = 0;
	short charisma = 0;
	short comeliness = 0;
	short ethics = 0;
	std::vector< std::tuple< uint8_t, std::string, std::string >> starting_clothes;
	std::string melee = "";
	std::string ranged = "";
	std::string ammo = "";
};

extern string_table_t first_names_male;
extern string_table_t first_names_female;
extern string_table_t last_names;

extern boost::container::flat_map<std::string, clothing_t> clothing_types;
extern std::vector<profession_t> starting_professions;

extern boost::container::flat_map<std::string, item_def_t> item_defs;
extern boost::container::flat_map<std::string, building_def_t> building_defs;
extern boost::container::flat_map<std::string, reaction_t> reaction_defs;
extern boost::container::flat_map<std::string, std::vector<std::string>> reaction_building_defs;

// Initializer
void load_raws();

// Tile accessors
uint8_t get_tile_type_index(const std::string name);
uint16_t get_tile_contents_index(const std::string name);

// Item creation
void spawn_item_on_ground(const int x, const int y, const int z, const std::string &tag, const std::size_t &material);
void spawn_item_in_container(const std::size_t container_id, const std::string &tag, const std::size_t &material);
void spawn_item_carried(const std::size_t holder_id, const std::string &tag, const std::size_t &material, const item_location_t &loc);

// Material accessors
std::size_t get_material_by_tag(const std::string &tag);
material_def_t& get_material(const std::size_t &idx);
std::string material_name(const std::size_t &id);
void get_strata_materials(std::vector<std::size_t> &soils, std::vector<std::size_t> &sedimintaries, std::vector<std::size_t> &igneouses, std::vector<std::size_t> &sands);
bool is_material_idx_valid(const std::size_t &id);

// Native population definition accessors
std::vector<native_population_t>& get_native_professions(const std::string &tag);

// Creature defs accessors
raw_species_t& get_species_def(const std::string &tag);
raw_creature_t& get_creature_def(const std::string &tag);
std::size_t get_species_defs_size();
std::string get_species_nth_tag(const int &n);

// Biome accessors
biome_type_t& get_biome_def(const std::size_t &index);
std::size_t get_plant_idx(const std::string &tag);
plant_t& get_plant_def(const std::size_t &index);
std::vector<plant_t>& get_plant_defs();
std::vector<biome_type_t>& get_biome_defs();

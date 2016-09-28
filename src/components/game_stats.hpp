#pragma once

#include <rltk.hpp>
#include <vector>
#include <unordered_map>

using namespace rltk;

inline short stat_modifier(const short &stat)
{
	if (stat == 1)
		return -5;
	return ((stat - 1) / 2) - 4;
}

struct skill_t {
	skill_t() {}
	skill_t(const int8_t lvl, const uint16_t &xp) : skill_level(lvl), experience_gained(xp) {}
	int8_t skill_level = 0;
	uint16_t experience_gained = 0;
};

struct game_stats_t {

	std::string profession_tag;
	short strength;
	short dexterity;
	short constitution;
	short intelligence;
	short wisdom;
	short charisma;
	short comeliness;
	short ethics;
	int age;
	std::unordered_map<std::string, skill_t> skills;

	game_stats_t() {}

	std::size_t serialization_identity = 8;

	void save(std::ostream &lbfile);
	static game_stats_t load(std::istream &lbfile);
	std::string strength_str();
	std::string dexterity_str();
	std::string constitution_str();
	std::string intelligence_str();
	std::string wisdom_str();
	std::string charisma_str();
	std::string comeliness_str();
	std::string ethics_str();
};

enum attributes_t { strength, dexterity, constitution, intelligence, wisdom, charisma, ethics };

const std::unordered_map<std::string, attributes_t> skill_table {
	{ "Mining", strength },
	{ "Lumberjacking", strength },
	{ "Construction", dexterity },
	{ "Carpentry", dexterity },
	{ "Masonry", strength },
	{ "Furnace Operation", dexterity },
	{ "Glassmaking", dexterity },
	{ "Metalworking", strength }
};

enum skill_roll_result_t { CRITICAL_FAIL, FAIL, SUCCESS, CRITICAL_SUCCESS };
constexpr int DIFFICULTY_TRIVIAL = 0;
constexpr int DIFFICULTY_EASY = 5;
constexpr int DIFFICULTY_AVERAGE = 10;
constexpr int DIFICULTY_TOUGH = 15;
constexpr int DIFFICULTY_CHALLENGING = 20;
constexpr int DIFFICULTY_FORMIDABLE = 25;
constexpr int DIFFICULTY_HEROIC = 30;
constexpr int DIFFICULTY_NEARLY_IMPOSSIBLE = 40;

int get_attribute_modifier_for_skill(const game_stats_t &stats, const std::string &skill);
int8_t get_skill_modifier(const game_stats_t &stats, const std::string &skill);
void gain_skill_from_success(const std::size_t settler_id, game_stats_t &stats, const std::string &skill, const int &difficulty, rltk::random_number_generator &rng);
skill_roll_result_t skill_roll(const std::size_t settler_id, game_stats_t &stats, rltk::random_number_generator &rng, const std::string skill_name, const int difficulty);

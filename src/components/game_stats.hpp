#pragma once

#include <vector>
#include <unordered_map>
#include <tuple>
#include "helpers/skill_t.hpp"
#include "../bengine/random_number_generator.hpp"

inline short stat_modifier(const short &stat)
{
	if (stat == 1)
		return -5;
	return ((stat - 1) / 2) - 4;
}

struct game_stats_t {

	std::string original_profession;
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
	{ "Metalworking", strength },
	{ "Swimming", strength },
	{ "Melee Attacks", strength },
	{ "Ranged Attacks", dexterity },
	{ "Negotiation", charisma },
	{ "Stealth", dexterity },
	{ "Medicine", intelligence },
	{ "Chemistry", intelligence },
	{ "Literacy", intelligence },
	{ "Acrobatics", dexterity },
	{ "Farming", wisdom }
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
int16_t get_skill_modifier(const game_stats_t &stats, const std::string &skill);
void gain_skill_from_success(const int settler_id, game_stats_t &stats, const std::string &skill, const int &difficulty, bengine::random_number_generator &rng);
skill_roll_result_t skill_roll(const int settler_id, game_stats_t &stats, bengine::random_number_generator &rng, const std::string skill_name, const int difficulty);
std::tuple<skill_roll_result_t, int, int> skill_roll_ext(const int settler_id, game_stats_t &stats, bengine::random_number_generator &rng, const std::string skill_name, const int difficulty);


#pragma once

#include <rltk.hpp>
#include <vector>
#include <unordered_map>

using namespace rltk;

constexpr short stat_modifier(const short &stat)
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

	void save(std::ostream &lbfile) {
		serialize(lbfile, profession_tag);
		serialize(lbfile, strength);
		serialize(lbfile, dexterity);
		serialize(lbfile, constitution);
		serialize(lbfile, intelligence);
		serialize(lbfile, wisdom);
		serialize(lbfile, charisma);
		serialize(lbfile, comeliness);
		serialize(lbfile, ethics);
		serialize(lbfile, age);
		serialize(lbfile, skills.size());
		for (auto it=skills.begin(); it!=skills.end(); ++it) {
			serialize(lbfile, it->first);
			serialize(lbfile, it->second.skill_level);
			serialize(lbfile, it->second.experience_gained);
		}
	}

	static game_stats_t load(std::istream &lbfile) {
		game_stats_t c;
		deserialize(lbfile, c.profession_tag);
		deserialize(lbfile, c.strength);
		deserialize(lbfile, c.dexterity);
		deserialize(lbfile, c.constitution);
		deserialize(lbfile, c.intelligence);
		deserialize(lbfile, c.wisdom);
		deserialize(lbfile, c.charisma);
		deserialize(lbfile, c.comeliness);
		deserialize(lbfile, c.ethics);
		deserialize(lbfile, c.age);
		std::size_t number_of_skills;
		deserialize(lbfile, number_of_skills);
		for (std::size_t i=0; i<number_of_skills; ++i) {
			std::string skill_name;
			int8_t skill_level;
			uint8_t experience_gained;

			deserialize(lbfile, skill_name);
			deserialize(lbfile, skill_level);
			deserialize(lbfile, experience_gained);
			c.skills[skill_name] = skill_t{skill_level, experience_gained};
		}
		return c;
	}
};

enum attributes_t { strength, dexterity, constitution, intelligence, wisdom, charisma, ethics };

const std::unordered_map<std::string, attributes_t> skill_table {
	{ "Mining", strength }
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

inline int get_attribute_modifier_for_skill(const game_stats_t &stats, const std::string &skill) {
	auto finder = skill_table.find(skill);
	if (finder != skill_table.end()) {
		switch (finder->second) {
			case strength : return stat_modifier(stats.strength);
			case dexterity : return stat_modifier(stats.dexterity);
			case constitution : return stat_modifier(stats.constitution);
			case intelligence : return stat_modifier(stats.intelligence);
			case wisdom : return stat_modifier(stats.wisdom);
			case charisma : return stat_modifier(stats.charisma);
			case ethics : return stat_modifier(stats.ethics);
		}
	} else {
		std::cout << "Warning: unknown skill [" << skill << "]\n";
		return 0;
	}
}

inline int8_t get_skill_modifier(const game_stats_t &stats, const std::string &skill) {
	auto finder = stats.skills.find(skill);
	if (finder != stats.skills.end()) {
		return finder->second.skill_level;
	} else {
		return 0;
	}
}

inline void gain_skill_from_success(game_stats_t &stats, const std::string &skill, const int &difficulty, rltk::random_number_generator &rng) {
	auto finder = stats.skills.find(skill);
	if (finder != stats.skills.end()) {
		finder->second.experience_gained += difficulty;
		const int gain_bonus_at = (finder->second.skill_level+1) * 100;
		if (finder->second.experience_gained > gain_bonus_at) {
			std::cout << "Skill improved!\n";
			finder->second.experience_gained = 0;
			++finder->second.skill_level;

			if (rng.roll_dice(1,6)==1) {
				auto relevant_attribute = skill_table.find(skill);
				if (relevant_attribute != skill_table.end()) {
					const int stat_gain_roll = rng.roll_dice(3,6);
					int attribute_target = 0;
					switch (relevant_attribute->second) {
						case strength : attribute_target = stats.strength; break;
						case dexterity : attribute_target = stats.dexterity; break;
						case constitution : attribute_target = stats.constitution; break;
						case intelligence : attribute_target = stats.intelligence; break;
						case wisdom : attribute_target = stats.wisdom; break;
						case charisma : attribute_target = stats.charisma; break;
						case ethics : attribute_target = stats.ethics; break;
					}
					if (stat_gain_roll < attribute_target) {
						std::cout << "Attribute improved!\n";
						switch (relevant_attribute->second) {
							case strength : ++stats.strength; break;
							case dexterity : ++stats.dexterity; break;
							case constitution : ++stats.constitution; break;
							case intelligence : ++stats.intelligence; break;
							case wisdom : ++stats.wisdom; break;
							case charisma : ++stats.charisma; break;
							case ethics : ++stats.ethics; break;
						}
					}
				}
			}
		}
	} else {
		stats.skills[skill] = skill_t{ 1, difficulty };
	}
}

inline skill_roll_result_t skill_roll(game_stats_t &stats, rltk::random_number_generator &rng, const std::string skill_name, const int difficulty) {
	const int luck_component = rng.roll_dice( 1, 20 );
	const int natural_ability = get_attribute_modifier_for_skill(stats, skill_name);
	const int8_t person_skill = get_skill_modifier(stats, skill_name);
	const int total = luck_component + natural_ability + person_skill;

	std::cout << skill_name << " roll, difficulty " << difficulty << ". 1d20 = " << luck_component << ", +" << natural_ability << " (ability) + " << +person_skill << " (skill) = " << total << "\n";

	if (luck_component == 1) {
		return CRITICAL_FAIL;
	} else if (luck_component == 20) {
		gain_skill_from_success(stats, skill_name, difficulty, rng);
		return CRITICAL_SUCCESS;
	} else if (total >= difficulty) {
		gain_skill_from_success(stats, skill_name, difficulty, rng);
		return SUCCESS;
	} else {
		return FAIL;
	}
}
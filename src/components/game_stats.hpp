#pragma once

#include <rltk.hpp>
#include <vector>

using namespace rltk;

constexpr short stat_modifier(const short &stat)
{
	if (stat == 1)
		return -5;
	return ((stat - 1) / 2) - 4;
}

struct skill_t
{
	std::string skill_name;
	int8_t skill_level;
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
	std::vector<skill_t> skills;

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
		for (const skill_t &skill : skills) {
			serialize(lbfile, skill.skill_name);
			serialize(lbfile, skill.skill_level);
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
			skill_t skill;
			deserialize(lbfile, skill.skill_name);
			deserialize(lbfile, skill.skill_level);
			c.skills.push_back(skill);
		}
		return c;
	}
};
#pragma once

#include "../game.h"
#include "tables.h"
#include <string>
#include <iostream>

using std::string;

namespace game_system
{

enum skill_test_result
{
	CRITICAL_FAIL, FAIL, SUCCESS, CRITICAL_SUCCCESS
};

inline skill_test_result skill_roll(const int skill_level, const int modifiers,
		const int difficulty)
{
	const int natural_roll = game_engine->rng.roll_dice(1, 20);
	const int roll = natural_roll + modifiers;

	//std::cout << "Skill roll! Level [" << skill_level << "], Modifiers: [" << modifiers << "], difficulty: [" << difficulty << "]. Natural " << natural_roll << " = " << roll << "\n";

	if (natural_roll == 20)
		return CRITICAL_SUCCCESS;
	if (natural_roll == 1)
		return CRITICAL_FAIL;
	if (roll < difficulty)
		return FAIL;
	return SUCCESS;
}

inline int get_skill_level(const game_stats_component * entity_stats,
		const string skill_name)
{
	// TODO: A more optimal search!
	int skill_level = 0;
	for (const skill_t &skill : entity_stats->skills)
	{
		if (skill_name == skill.skill_name)
			skill_level = skill.skill_level;
	}
	return skill_level;
}

inline int get_skill_modifier(const game_stats_component * entity_stats,
		const string skill_name)
{
	int modifier = 0;

	if (skill_name == "Construction")
		modifier = stat_modifier(entity_stats->dexterity);
	if (skill_name == "Lumberjack")
		modifier = stat_modifier(entity_stats->strength);
	if (skill_name == "Self Heal")
		modifier = stat_modifier(entity_stats->constitution);

	return modifier;
}

inline skill_test_result skill_roll(const int entity_id,
		const string skill_name, int difficulty)
{
	const game_stats_component * entity_stats = ECS->find_entity_component<
			game_stats_component>(entity_id);
	if (entity_stats == nullptr)
	{
		std::cout << "Warning: no game_stats attached to entity.\n";
		return FAIL;
	}
	const int skill_level = get_skill_level(entity_stats, skill_name);
	const int modifiers = get_skill_modifier(entity_stats, skill_name);
	return skill_roll(skill_level, modifiers, difficulty);

}

}

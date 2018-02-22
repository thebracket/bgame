#pragma once

#include <array>
#include "position.hpp"
#include "../components/helpers/reaction_task_t.hpp"
#include "../components/helpers/building_designation_t.hpp"
#include "../systems/helpers/pathfinding.hpp"

constexpr int NUMBER_OF_JOB_CATEGORIES = 10;
constexpr int JOB_MINING = 0;
constexpr int JOB_CHOPPING = 1;
constexpr int JOB_CONSTRUCTION = 2;
constexpr int JOB_CARPENTRY = 3;
constexpr int JOB_MASONRY = 4;
constexpr int JOB_HUNTING = 5;
constexpr int JOB_BUTCHER = 6;
constexpr int JOB_GUARDING = 7;
constexpr int JOB_HAULING = 8;
constexpr int JOB_FARMING = 9;

enum job_major_t { JOB_IDLE, JOB_ROGUE_GOTO, JOB_EQUIP_MELEE,
		JOB_ROGUE_BASH, JOB_EQUIP_RANGED, JOB_EQUIP_AMMO, JOB_ROGUE_SHOOT, JOB_EQUIP_ARMOR,
		JOB_DECONSTRUCT, JOB_DEMOLISH, JOB_TIDY };
enum job_minor_t { 
	JM_NONE,
	JM_FIND_MELEE_WEAPON, JM_GO_TO_MELEE_WEAPON, JM_COLLECT_MELEE_WEAPON, // Equip a melee weapon
	JM_FIND_RANGED_WEAPON, JM_GO_TO_RANGED_WEAPON, JM_COLLECT_RANGED_WEAPON, // Equip a ranged weapon
	JM_FIND_AMMO, JM_GO_TO_AMMO, JM_COLLECT_AMMO, // Equip a ranged weapon
	JM_FIND_ARMOR, JM_GO_TO_ARMOR, JM_COLLECT_ARMOR, // Equip armor
	JM_FIND_DECONSTRUCT, JM_GO_TO_DECONSTRUCT, JM_DECONSTRUCT, // Deconstruction
	JM_FIND_DEMOLISH, JM_GO_TO_DEMOLISH, JM_DEMOLISH, // Demolition
    JM_FIND_TIDY, JM_GO_TO_TIDY, JM_COLLECT_TIDY, JM_FIND_STOCKPILE, JM_GO_TO_STOCKPILE, JM_STORE_ITEM // Tidying/stockpiling
};

struct settler_ai_t {

	std::array<bool, NUMBER_OF_JOB_CATEGORIES> permitted_work;
	int shift_id = 0;

	job_major_t job_type_major = JOB_IDLE;
	std::string job_status = "";
	job_minor_t job_type_minor = JM_NONE;
	int target_x = 0;
	int target_y = 0;
	int target_z = 0;
	int target_id = 0;
	bool has_building_target = false;
	building_designation_t building_target;
	bool has_reaction_target = false;
	reaction_task_t reaction_target;
	std::size_t current_tool = 0;

	// Non-persistent
	std::shared_ptr<navigation_path_t> current_path;
	std::size_t targeted_hostile = 0;

	settler_ai_t() {
		std::fill(permitted_work.begin(), permitted_work.end(), true);
	}	
};

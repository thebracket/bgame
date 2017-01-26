#pragma once

#include <rltk.hpp>
#include <array>
#include "position.hpp"
#include "designations.hpp"

using namespace rltk;

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

enum job_major_t { JOB_IDLE, JOB_SLEEP, JOB_MINE, JOB_CHOP, JOB_CONST, JOB_REACTION, JOB_ROGUE_GOTO, JOB_EQUIP_MELEE, 
		JOB_ROGUE_BASH, JOB_EQUIP_RANGED, JOB_EQUIP_AMMO, JOB_HUNT, JOB_BUTCHERING, JOB_ROGUE_SHOOT, JOB_EQUIP_ARMOR,
		JOB_GUARD, JOB_DECONSTRUCT, JOB_DEMOLISH, JOB_TIDY, JOB_HARVEST, JOB_ARCHITECT, JOB_PULL_LEVER };
enum job_minor_t { 
	JM_NONE,
	JM_FIND_BED, JM_GO_TO_BED, JM_SLEEP,				// Bed-time steps
	JM_FIND_PICK, JM_GO_TO_PICK, JM_COLLECT_PICK, JM_GO_TO_SITE, JM_DIG, JM_DROP_PICK,	// Mining steps
	JM_FIND_AXE, JM_GO_TO_AXE, JM_COLLECT_AXE, JM_FIND_TREE, JM_GO_TO_TREE, JM_CHOP, JM_DROP_AXE,	// Tree cutting steps
	JM_SELECT_COMPONENT, JM_GO_TO_COMPONENT, JM_COLLECT_COMPONENT, JM_GO_TO_BUILDING, JM_DROP_COMPONENT, JM_ASSEMBLE, // Buildings
	JM_SELECT_INPUT, JM_GO_TO_INPUT, JM_COLLECT_INPUT, JM_GO_TO_WORKSHOP, JM_DROP_INPUT, JM_REACT, // Reactions
	JM_FIND_MELEE_WEAPON, JM_GO_TO_MELEE_WEAPON, JM_COLLECT_MELEE_WEAPON, // Equip a melee weapon
	JM_FIND_RANGED_WEAPON, JM_GO_TO_RANGED_WEAPON, JM_COLLECT_RANGED_WEAPON, // Equip a ranged weapon
	JM_FIND_AMMO, JM_GO_TO_AMMO, JM_COLLECT_AMMO, // Equip a ranged weapon
	JM_HUNT_FIND_TARGET, JM_HUNT, // Hunting
	JM_BUTCHER_FIND_CORPSE, JM_BUTCHER_GO_TO_CORPSE, JM_BUTCHER_COLLECT_CORPSE, JM_BUTCHER_GO_TO_SHOP, JM_BUTCHER_CHOP, // Butchering
	JM_FIND_ARMOR, JM_GO_TO_ARMOR, JM_COLLECT_ARMOR, // Equip armor
	JM_FIND_GUARDPOST, JM_GO_TO_GUARDPOST, JM_GUARD, // Guarding
	JM_FIND_DECONSTRUCT, JM_GO_TO_DECONSTRUCT, JM_DECONSTRUCT, // Deconstruction
	JM_FIND_DEMOLISH, JM_GO_TO_DEMOLISH, JM_DEMOLISH, // Demolition
    JM_FIND_TIDY, JM_GO_TO_TIDY, JM_COLLECT_TIDY, JM_FIND_STOCKPILE, JM_GO_TO_STOCKPILE, JM_STORE_ITEM, // Tidying/stockpiling
    JM_FIND_HARVEST, JM_GO_TO_HARVEST, JM_DO_HARVEST, // Harvesting
	JM_ARCHITECT_GOTOBLOCK, JM_ARCHITECT_COLLECTBLOCK, JM_ARCHITECT_GOTOSITE, JM_ARCHITECT_BUILD, // Architecture
    JM_GO_TO_LEVER, JM_PULLIT // Levers
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
	std::shared_ptr<rltk::navigation_path<position_t>> current_path;
	std::size_t targeted_hostile = 0;

	settler_ai_t() {
		std::fill(permitted_work.begin(), permitted_work.end(), true);
	}

	std::string xml_identity = "settler_ai_t";

	void to_xml(xml_node * c);
	void from_xml(xml_node * c);
};
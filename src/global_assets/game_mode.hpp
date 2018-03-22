#pragma once

enum game_master_mode_t {
	PLAY, DESIGN, UNITS, SETTLER, WORKFLOW, ROGUE, CIVS, CIV_NEGOTIATE,
	STANDING_ORDERS, TILEMENU, SENTIENT_INFO, GRAZER_INFO, WISHMODE, TRIGGER_MANAGEMENT,
	ITEM_INFO, JOB_CENTER_MODE, LOOK_MODE, BUILDING_INFO
};
enum game_design_mode_t { DIGGING, BUILDING, CHOPPING, GUARDPOINTS, STOCKPILES, HARVEST, ARCHITECTURE };

extern game_master_mode_t game_master_mode;
extern game_design_mode_t game_design_mode;

extern size_t current_stockpile;

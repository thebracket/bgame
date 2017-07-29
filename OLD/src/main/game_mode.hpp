#pragma once

enum game_master_mode_t { PLAY, DESIGN, UNITS, SETTLER, WORKFLOW, ROGUE, CIVS, CIV_NEGOTIATE, 
	STANDING_ORDERS, TILEMENU, SENTIENT_INFO, GRAZER_INFO, WISHMODE, TRIGGER_MANAGEMENT };
enum game_design_mode_t { DIGGING, BUILDING, CHOPPING, GUARDPOINTS, STOCKPILES, HARVEST, ARCHITECTURE };
enum game_mining_mode_t { DIG, CHANNEL, RAMP, UP, DOWN, UPDOWN, MINING_DELETE };

extern game_master_mode_t game_master_mode;
extern game_design_mode_t game_design_mode;
extern game_mining_mode_t game_mining_mode;
extern bool quitting;

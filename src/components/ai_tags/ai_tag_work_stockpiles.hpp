#pragma once

#include "../../systems/helpers/pathfinding.hpp"
#include <memory>

struct ai_tag_work_stockpiles_t {

	enum stockpile_steps { FIND_ITEM, GOTO_ITEM, GOTO_STOCKPILE, DROP_ITEM };

	stockpile_steps step = FIND_ITEM;
	std::size_t tool_id = 0;
	int destination = 0;
	std::shared_ptr<navigation_path_t> current_path; // Not serialized
};


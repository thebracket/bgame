#pragma once

#include "../../systems/helpers/pathfinding.hpp"
#include <memory>

struct ai_tag_work_deconstruct {

	enum deconstruct_steps { FIND_TARGET, GOTO_TARGET, UNBUILD };

	deconstruct_steps step = FIND_TARGET;
	std::size_t target_building = 0;

	std::shared_ptr<navigation_path_t> current_path; // Not serialized
};


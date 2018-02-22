#pragma once

#include "../../systems/helpers/pathfinding.hpp"
#include <memory>

struct ai_tag_work_farm_fertilize {

    enum fertilize_steps { FIND_HOE, FETCH_HOE, FIND_FERTILZER, FETCH_FERTILIZER, FIND_TARGET, FETCH_TARGET, FERILIZE };

	fertilize_steps step = FIND_HOE;
	std::size_t tool_id = 0;
	std::size_t fertilizer_id = 0;
	std::shared_ptr<navigation_path_t> current_path; // Not serialized

};


#pragma once

#include "../../systems/helpers/pathfinding.hpp"
#include <memory>

struct ai_tag_work_farm_water {

    enum water_steps { FIND_HOE, FETCH_HOE, FIND_TARGET, FETCH_TARGET, WATER };

	water_steps step = FIND_HOE;
	std::size_t tool_id = 0;
	std::shared_ptr<navigation_path_t> current_path; // Not serialized
};

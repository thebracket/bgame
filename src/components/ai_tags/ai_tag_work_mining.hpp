#pragma once

#include <memory>
#include "../../systems/helpers/pathfinding.hpp"

struct ai_tag_work_miner {

    enum mining_steps { GET_PICK, FETCH_PICK, GOTO_SITE, DIG };

    mining_steps step = GET_PICK;
	std::size_t tool_id = 0;
	int target_tile = 0;

	std::shared_ptr<navigation_path_t> current_path; // Not serialized

};


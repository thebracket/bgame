#pragma once

#include <memory>
#include "../../systems/helpers/pathfinding.hpp"

struct ai_tag_work_architect {

    enum architect_steps { GOTO_BLOCK, COLLECT_BLOCK, GOTO_SITE, BUILD };

    architect_steps step = GOTO_BLOCK;
    std::size_t current_tool = 0;
	std::size_t target_tile = 0;
    std::shared_ptr<navigation_path_t> current_path; // Not serialized
};


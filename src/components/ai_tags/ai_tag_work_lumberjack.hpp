#pragma once

#include "../position.hpp"
#include "../../bengine/path_finding.hpp"
#include <memory>

struct ai_tag_work_lumberjack {

    enum lumberjack_steps { GET_AXE, FETCH_AXE, FIND_TREE, GOTO_TREE, CHOP };

    lumberjack_steps step = GET_AXE;
    std::size_t tool_id = 0;
    std::size_t target_tree = 0;
    int target_x, target_y, target_z;

    std::shared_ptr<bengine::navigation_path<position_t>> current_path; // Not serialized
};



#pragma once

#include "../position.hpp"
#include "../../components/helpers/reaction_task_t.hpp"
#include <memory>

struct ai_tag_work_order {

    enum work_steps { FIND_JOB, SELECT_INPUT, GO_TO_INPUT, COLLECT_INPUT, GO_TO_WORKSHOP, DROP_INPUT, REACT };

    work_steps step = FIND_JOB;
    reaction_task_t reaction_target;
    std::size_t current_tool;

    std::shared_ptr<navigation_path_t> current_path; // Not serialized
};


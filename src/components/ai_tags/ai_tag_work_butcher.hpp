#pragma once

#include "../../systems/helpers/pathfinding.hpp"
#include <memory>

struct ai_tag_work_butcher {

    enum butcher_steps { FIND_CORPSE, COLLECT_CORPSE, GO_TO_SHOP, CHOP };

    butcher_steps step = FIND_CORPSE;
    std::size_t target_corpse = 0;

    std::shared_ptr<navigation_path_t> current_path; // Not serialized
};


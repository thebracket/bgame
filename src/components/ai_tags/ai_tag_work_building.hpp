#pragma once

#include "../helpers/building_designation_t.hpp"
#include "../../systems/helpers/pathfinding.hpp"

struct ai_tag_work_building {

    enum building_steps { SELECT_BUILDING, SELECT_COMPONENT, GO_TO_COMPONENT, COLLECT_COMPONENT, GO_TO_BUILDING, DROP_COMPONENT, ASSEMBLE };

    building_steps step = SELECT_BUILDING;
    building_designation_t building_target{};
    int current_tool = 0;

    std::shared_ptr<navigation_path_t> current_path; // Not serialized
};


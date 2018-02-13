#pragma once

#include "../position.hpp"
#include "../../bengine/path_finding.hpp"
#include <memory>

struct ai_tag_work_hunting {

    enum hunting_steps { FIND_TARGET, HUNT };

    hunting_steps step = FIND_TARGET;

    std::shared_ptr<bengine::navigation_path<position_t>> current_path; // Not serialized

};

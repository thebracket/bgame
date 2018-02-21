#pragma once

#include "../position.hpp"
#include "../../bengine/path_finding.hpp"
#include <memory>

struct ai_tag_work_hunting {

    enum hunting_steps { CHECK_STATUS, FIND_TARGET, HUNT };

    hunting_steps step = CHECK_STATUS;

    std::shared_ptr<bengine::navigation_path<position_t>> current_path; // Not serialized

};

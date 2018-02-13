#pragma once

#include "../position.hpp"
#include "../../bengine/path_finding.hpp"
#include <memory>

struct ai_tag_work_guarding {

    enum guard_steps { GOTO_POST, GUARD };

    guard_steps step = GOTO_POST;
    bool has_post = false;
    position_t guard_post;

    std::shared_ptr<bengine::navigation_path<position_t>> current_path; // Not serialized
};


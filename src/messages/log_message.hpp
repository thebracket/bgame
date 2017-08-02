#pragma once

#include "../bengine/ecs.hpp"
#include "../bengine/rexspeeder.hpp"

struct log_message : public bengine::base_message_t {
    log_message() {}
    log_message(std::vector<xp::vchar> &line) : chars(line) {}
    std::vector<xp::vchar> chars;
};

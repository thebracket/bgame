#pragma once

#include <rltk.hpp>

struct log_message : public rltk::base_message_t {
    log_message() {}
    log_message(std::vector<rltk::vchar> &line) : chars(line) {}
    std::vector<rltk::vchar> chars;
};

#pragma once

#include "name.hpp"
#include "../bengine/rexspeeder.hpp"
#include "../bengine/color_t.hpp"
#include <vector>

struct log_line_t {
	log_line_t() = default;
    explicit log_line_t(const std::vector<xp::vchar> new_chars) : chars(new_chars) {}

    int age = 0;
    std::vector<xp::vchar> chars;	
};

struct logger_t {
	logger_t() = default;
    std::vector<log_line_t> lines;	
};

struct LOG {
    std::vector<xp::vchar> chars;
    bengine::color_t fg{1.0f, 1.0f, 1.0f};

    LOG * text(const std::string &txt);
    inline LOG * col(const bengine::color_t &col) { fg = col; return this; }
    LOG * settler_name(const int &entity_id);
    LOG * settler_name_t(const name_t &name);
    LOG * sentient_name(const int &entity_id);
    LOG * other_name(const int &entity_id);
    LOG * civ_name(const std::size_t &civ_id);
};

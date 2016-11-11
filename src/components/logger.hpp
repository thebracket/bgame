#pragma once

#include <rltk.hpp>
#include "name.hpp"

struct log_line_t {
    log_line_t() {}
    log_line_t(std::vector<rltk::vchar> new_chars) : chars(new_chars) {}

    int age = 0;
    std::vector<rltk::vchar> chars;

    void to_xml(rltk::xml_node * c);
    void from_xml(rltk::xml_node * c);
};

struct logger_t {
	logger_t() {}
    std::vector<log_line_t> lines;

    std::string xml_identity = "logger_t";

    void to_xml(xml_node * c);
    void from_xml(xml_node * c);
};

struct LOG {
    std::vector<rltk::vchar> chars;
    rltk::color_t fg = rltk::colors::WHITE;

    LOG * text(const std::string &txt);
    inline LOG * col(const rltk::color_t &col) { fg = col; return this; }
    LOG * settler_name(const std::size_t &entity_id);
    LOG * settler_name_t(const name_t &name);
    LOG * sentient_name(const std::size_t &entity_id);
    LOG * other_name(const std::size_t &entity_id);
    LOG * civ_name(const std::size_t &civ_id);
};
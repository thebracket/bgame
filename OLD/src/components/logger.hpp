#pragma once

#include <rltk.hpp>
#include "name.hpp"
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/polymorphic.hpp>

struct log_line_t {
    log_line_t() {}
    log_line_t(std::vector<rltk::vchar> new_chars) : chars(new_chars) {}

    int age = 0;
    std::vector<rltk::vchar> chars;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( age, chars ); // serialize things by passing them to the archive
	}
};

struct logger_t {
	logger_t() {}
    std::vector<log_line_t> lines;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( lines ); // serialize things by passing them to the archive
	}
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

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<logger_t>>)
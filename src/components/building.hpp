#pragma once

#include <rltk.hpp>
#include <vector>
#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/utility.hpp>

using namespace rltk;

struct building_t {
    building_t(const std::string ntag, const int w, const int h, const std::vector<rltk::vchar> &g, const std::vector<rltk::vchar> &ga, const bool comp, const std::size_t owner=0) :
        tag(ntag), width(w), height(h), glyphs(g), glyphs_ascii(ga), complete(comp), civ_owner(owner) {}

    std::string tag;
    int width, height;
    std::vector<rltk::vchar> glyphs;
	std::vector<rltk::vchar> glyphs_ascii;
    bool complete = false;
    std::vector<std::pair<std::string, std::size_t>> built_with;
    std::size_t civ_owner=0;

	building_t() {}

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( tag, width, height, glyphs, glyphs_ascii, complete, built_with, civ_owner ); // serialize things by passing them to the archive
	}

};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<building_t>>)

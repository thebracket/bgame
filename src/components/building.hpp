#pragma once

#include <vector>
#include <cereal/archives/xml.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/utility.hpp>
#include "../bengine/ecs_impl.hpp"
#include "../bengine/rexspeeder.hpp"

struct building_t {
    building_t(const std::string ntag, const int w, const int h, const std::vector<xp::vchar> &g,
			   const std::vector<xp::vchar> &ga, const bool comp, const std::size_t owner, uint8_t maxhp,
				uint8_t hp, int vox) :
        tag(ntag), width(w), height(h), glyphs(g), glyphs_ascii(ga), complete(comp), civ_owner(owner),
		max_hit_points(maxhp), hit_points(hp), vox_model(vox)
	{}

    std::string tag;
    int width, height;
    std::vector<xp::vchar> glyphs;
	std::vector<xp::vchar> glyphs_ascii;
    bool complete = false;
    std::vector<std::pair<std::string, std::size_t>> built_with;
    std::size_t civ_owner=0;
	uint8_t max_hit_points=10;
	uint8_t hit_points=10;
    int vox_model = 0;

	building_t() {}

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( tag, width, height, glyphs, glyphs_ascii, complete, built_with, civ_owner, max_hit_points, hit_points, vox_model ); // serialize things by passing them to the archive
	}

};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<building_t>>)

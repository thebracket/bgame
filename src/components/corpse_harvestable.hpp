#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

using namespace rltk;

struct corpse_harvestable {
	corpse_harvestable() {}
    corpse_harvestable(const std::string &tag) : creature_tag(tag) {}
    uint32_t ticks_since_death = 0;
    std::string creature_tag = "";
	bool claimed = false;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( ticks_since_death, creature_tag, claimed ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<corpse_harvestable>>)
#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../bengine/ecs.hpp"

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

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<corpse_harvestable>>)
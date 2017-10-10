#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../bengine/ecs.hpp"

struct corpse_settler {
	corpse_settler() {}
	corpse_settler(const std::string cause) : cause_of_death(cause) {}
    uint32_t ticks_since_death = 0;
	std::string cause_of_death = "";

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( ticks_since_death, cause_of_death ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<corpse_settler>>)

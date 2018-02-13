#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../../bengine/ecs_impl.hpp"

struct construct_door_t {
	bool locked = false;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( locked ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<construct_door_t>>)

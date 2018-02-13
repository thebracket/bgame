#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../../bengine/ecs_impl.hpp"

struct item_ranged_t {

	template<class Archive>
	void serialize(Archive & archive)
	{
		// Nothing to save
		//archive( );
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_ranged_t>>)

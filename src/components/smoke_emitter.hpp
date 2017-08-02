#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../bengine/ecs_impl.hpp"

struct smoke_emitter_t {
	smoke_emitter_t() {}

	template<class Archive>
	void serialize(Archive & archive)
	{
		//archive(  ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<smoke_emitter_t>>)
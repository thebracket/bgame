#pragma once

#include "../bengine/ecs_impl.hpp"
#include <cereal/archives/xml.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

struct grazer_ai {
	template<class Archive>
	void serialize(Archive & archive)
	{
		//archive(  ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<grazer_ai>>)

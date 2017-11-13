#pragma once

#include "../bengine/ecs_impl.hpp"
#include "../stdafx.h"
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

struct item_stored_t {
	std::size_t stored_in;
	item_stored_t() {}
	item_stored_t(const std::size_t carrier) : stored_in(carrier) {}

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( stored_in ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_stored_t>>)
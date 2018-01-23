#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../../bengine/ecs_impl.hpp"

struct item_wear_t {

	item_wear_t() {}
	item_wear_t(const uint8_t w) : wear(w) {}
	uint8_t wear = 100;

	template<class Archive>
	void serialize(Archive & archive)
	{
		// Nothing to save
		archive(wear);
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_wear_t>>)

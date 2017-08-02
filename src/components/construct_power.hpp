#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../bengine/ecs_impl.hpp"

struct construct_power_t {
	int storage_capacity;
	int generation_always;
	int generation_solar;

	construct_power_t() {}
	construct_power_t(const int store, const int gen_all, const int gen_solar) : storage_capacity(store), generation_always(gen_all), generation_solar(gen_solar) {}

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( storage_capacity, generation_always, generation_solar ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<construct_power_t>>)

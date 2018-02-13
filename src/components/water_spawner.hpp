#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../bengine/ecs_impl.hpp"

struct water_spawner_t {
	water_spawner_t() = default;
	explicit water_spawner_t(const uint8_t &t) noexcept : spawner_type(t) {}

    uint8_t spawner_type = 0;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( spawner_type ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<water_spawner_t>>)
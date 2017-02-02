#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

using namespace rltk;

struct water_spawner_t {
	water_spawner_t() {}
	water_spawner_t(const uint8_t &t) : spawner_type(t) {}

    uint8_t spawner_type = 0;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( spawner_type ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<water_spawner_t>>)
#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

using namespace rltk;

struct construct_provides_sleep_t {
	bool claimed = false;

	construct_provides_sleep_t() {}

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( claimed ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<construct_provides_sleep_t>>)

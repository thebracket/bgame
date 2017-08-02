#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

using namespace rltk;

struct falling_t {
	falling_t() {}
    falling_t(const int &d) : distance(d) {}
    int distance = 0;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( distance ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<falling_t>>)

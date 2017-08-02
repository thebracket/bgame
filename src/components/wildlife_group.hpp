#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

using namespace rltk;

struct wildlife_group {
	wildlife_group() {}
	wildlife_group(uint8_t &id) : group_id(id) {}
    uint8_t group_id;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( group_id ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<wildlife_group>>)
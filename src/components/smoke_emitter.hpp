#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

using namespace rltk;

struct smoke_emitter_t {
	smoke_emitter_t() {}

	template<class Archive>
	void serialize(Archive & archive)
	{
		//archive(  ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<smoke_emitter_t>>)
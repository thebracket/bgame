#pragma once

#include <rltk.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

using namespace rltk;

struct grazer_ai {
	grazer_ai() {}

	template<class Archive>
	void serialize(Archive & archive)
	{
		//archive(  ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<grazer_ai>>)

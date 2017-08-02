#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../bengine/ecs_impl.hpp"

struct name_t {
	std::string first_name;
	std::string last_name;

	name_t() {}
	name_t(const std::string fn, const std::string ln) : first_name(fn), last_name(ln) {}

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( first_name, last_name ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<name_t>>)
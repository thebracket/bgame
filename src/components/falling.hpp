#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../bengine/ecs_impl.hpp"


struct falling_t {
	falling_t() = default;
    explicit falling_t(const int &d) noexcept : distance(d) {}
    int distance = 0;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( distance ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<falling_t>>)

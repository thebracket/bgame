#pragma once

#include "../position.hpp"
#include <unordered_map>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../../bengine/ecs_impl.hpp"

struct architecture_designations_t {

	architecture_designations_t() = default;

	std::unordered_map<std::size_t, uint8_t> architecture;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(architecture); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<architecture_designations_t>>)

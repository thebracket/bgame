#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../../bengine/ecs_impl.hpp"
#include <string>

struct item_creator_t {

	item_creator_t() = default;
	item_creator_t(const std::size_t id, const std::string name) : creator_id(id), creator_name(name) {}
	std::size_t creator_id = 0;
	std::string creator_name = "";

	template<class Archive>
	void serialize(Archive & archive)
	{
		// Nothing to save
		archive(creator_id, creator_name);
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_creator_t>>)

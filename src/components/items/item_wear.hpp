#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../../bengine/ecs_impl.hpp"
#include <string>

struct item_wear_t {

	item_wear_t() {}
	item_wear_t(const uint8_t w) : wear(w) {}
	uint8_t wear = 100;

	std::string get_wear_text() {
		if (wear < 10) return std::string("falling apart");
		if (wear < 30) return std::string("tattered");
		if (wear < 50) return std::string("worn");
		if (wear < 75) return std::string("lightly used");
		return std::string("pristine");
	}

	template<class Archive>
	void serialize(Archive & archive)
	{
		// Nothing to save
		archive(wear);
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<item_wear_t>>)

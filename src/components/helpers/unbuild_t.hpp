#pragma once

#include <istream>
#include <ostream>
#include <cereal/cereal.hpp>

struct unbuild_t {
	unbuild_t() = default;
	unbuild_t(const bool &build, const std::size_t id) noexcept : is_building(build), building_id(id) {}
	bool is_building = true;
	std::size_t building_id;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( is_building, building_id ); // serialize things by passing them to the archive
	}
};

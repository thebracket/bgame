#pragma once

struct unbuild_t {
	unbuild_t() = default;
	unbuild_t(const bool &build, const std::size_t id) noexcept : is_building(build), building_id(id) {}
	bool is_building = true;
	std::size_t building_id;
};

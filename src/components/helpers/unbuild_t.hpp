#pragma once

#include <istream>
#include <ostream>

struct unbuild_t {
	unbuild_t() {}
	unbuild_t(const bool &build, const std::size_t id) : is_building(build), building_id(id) {}
	bool is_building = true;
	std::size_t building_id;

    void save(std::ostream &f);
    void load(std::istream &f);
};

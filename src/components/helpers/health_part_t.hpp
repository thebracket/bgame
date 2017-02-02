#pragma once

#include <istream>
#include <ostream>
#include <rltk.hpp>
#include <cereal/cereal.hpp>

struct health_part_t {
	std::string part = "";
	int max_hitpoints = 0;
	int current_hitpoints = 0;
	int size = 0;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( part, max_hitpoints, current_hitpoints, size ); // serialize things by passing them to the archive
	}
};
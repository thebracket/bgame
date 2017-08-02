#pragma once

#include <array>
#include <istream>
#include <ostream>
#include <cereal/cereal.hpp>
#include <cereal/types/array.hpp>

enum shift_type_t { SLEEP_SHIFT, LEISURE_SHIFT, WORK_SHIFT };

struct shift_t {
	std::string shift_name;
	std::array<shift_type_t, 24> hours;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( shift_name, hours ); // serialize things by passing them to the archive
	}
};

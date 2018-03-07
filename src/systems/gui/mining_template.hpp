#pragma once

#include <cereal/archives/xml.hpp>
#include <cereal/types/vector.hpp>
#include <string>

struct mining_target_t {
	int x, y, z;
	uint8_t mode;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(x, y, z, mode); // serialize things by passing them to the archive
	}
};

struct mining_template_t
{
	std::string title;
	std::vector<mining_target_t> targets;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(title, targets); // serialize things by passing them to the archive
	}
};

#pragma once

#include <cereal/archives/xml.hpp>
#include <cereal/types/vector.hpp>
#include <string>

struct architecture_target {
	int x, y, z;
	uint8_t mode;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(x, y, z, mode); // serialize things by passing them to the archive
	}
};

struct architecture_template_t
{
	std::string title;
	std::vector<architecture_target> targets;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(title, targets); // serialize things by passing them to the archive
	}
};

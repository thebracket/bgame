#pragma once

#include <cereal/cereal.hpp>

enum material_def_spawn_type_t { NO_SPAWN_TYPE, CLUSTER_ROCK, ROCK, SOIL, SAND, METAL, SYNTHETIC, ORGANIC, LEATHER, FOOD, SPICE, BLIGHT };

struct reaction_input_t {
	std::string tag = "";
	std::size_t required_material = 0;
	material_def_spawn_type_t required_material_type = NO_SPAWN_TYPE;
	int quantity = 0;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( tag, required_material, required_material_type, quantity ); // serialize things by passing them to the archive
	}
};

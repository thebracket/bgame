#pragma once

#include <istream>
#include <ostream>
//#include <rltk.hpp>
#include <cereal/cereal.hpp>

enum material_def_spawn_type_t { no_spawn_type, cluster_rock, rock, soil, sand, metal, synthetic, organic, leather, food, spice, blight };

struct reaction_input_t {
	std::string tag = "";
	std::size_t required_material = 0;
	material_def_spawn_type_t required_material_type = no_spawn_type;
	int quantity = 0;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( tag, required_material, required_material_type, quantity ); // serialize things by passing them to the archive
	}
};

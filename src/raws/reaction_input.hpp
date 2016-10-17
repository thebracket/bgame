#pragma once

#include <istream>
#include <ostream>
#include <boost/optional.hpp>
#include "../utils/serialization_wrapper.hpp"

enum material_def_spawn_type_t { cluster_rock, rock, soil, sand, metal, synthetic, organic, leather };

struct reaction_input_t {
	std::string tag = "";
	boost::optional<std::size_t> required_material;
	boost::optional<material_def_spawn_type_t> required_material_type;
	int quantity = 0;

	void save(std::ostream &f) {
		Serialize(f, tag, required_material, required_material_type, quantity);
	}

	void load(std::istream f) {
		Deserialize(f, tag, required_material, required_material_type, quantity);
	}
};

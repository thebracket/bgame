#pragma once

enum material_def_spawn_type_t { cluster_rock, rock, soil, sand, metal, synthetic, organic, leather };

struct reaction_input_t {
	std::string tag = "";
	boost::optional<std::size_t> required_material;
	boost::optional<material_def_spawn_type_t> required_material_type;
	int quantity = 0;
};

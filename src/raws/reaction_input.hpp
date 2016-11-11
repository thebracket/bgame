#pragma once

#include <istream>
#include <ostream>
#include <boost/optional.hpp>
#include <rltk.hpp>

enum material_def_spawn_type_t { cluster_rock, rock, soil, sand, metal, synthetic, organic, leather };

struct reaction_input_t {
	std::string tag = "";
	boost::optional<std::size_t> required_material;
	boost::optional<material_def_spawn_type_t> required_material_type;
	int quantity = 0;

	void to_xml(rltk::xml_node * c) {
		rltk::component_to_xml(c,
			std::make_pair("tag", tag),
			std::make_pair("required_material", required_material),
			std::make_pair("required_material_type", required_material_type),
			std::make_pair("quantity", quantity)
		);
	}

	void from_xml(rltk::xml_node * c) {
		tag = c->val<std::string>("tag");
		required_material = c->val<std::size_t>("required_material");
		required_material_type = (material_def_spawn_type_t)c->val<int>("required_material_type");
		quantity = c->val<int>("quantity");
	}

};

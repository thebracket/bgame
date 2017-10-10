#pragma once

#include "../raws/reaction_input.hpp"
#include <string>
#include <vector>

namespace buildings {
	struct available_building_t {
		available_building_t() {}
		available_building_t(const std::string &n, const std::string &t) : name(n), tag(t) {}

		std::string name = "";
		std::string tag = "";
		std::vector<reaction_input_t> components;
		int width = 0;
		int height = 0;
		//std::vector<bengine::vchar> glyphs;
		//std::vector<bengine::vchar> glyphs_ascii;
		int n_existing = 0;
		std::string get_name() const noexcept {
			return name + std::string(" (") + std::to_string(n_existing) + std::string(")");
		}
		bool structure = false;
	};

	extern bool has_build_mode_building;
	extern available_building_t build_mode_building;
}
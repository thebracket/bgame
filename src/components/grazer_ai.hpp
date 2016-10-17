#pragma once

#include <rltk.hpp>
#include "../utils/serialization_wrapper.hpp"

using namespace rltk;

struct grazer_ai {
	grazer_ai() {}
	grazer_ai(const int init_mod) : initiative_modifier(init_mod) {}
	int initiative = 0;
	int initiative_modifier = 0;

	std::size_t serialization_identity = 20;

	void save(std::ostream &lbfile);
	static grazer_ai load(std::istream &lbfile);
};
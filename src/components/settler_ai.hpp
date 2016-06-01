#pragma once

#include <rltk.hpp>
#include <array>

using namespace rltk;

constexpr int NUMBER_OF_JOB_CATEGORIES = 1;
constexpr int JOB_MINING = 0;

struct settler_ai_t {

	int initiative = 0;
	std::array<bool, NUMBER_OF_JOB_CATEGORIES> permitted_work;

	settler_ai_t() {
		std::fill(permitted_work.begin(), permitted_work.end(), true);
	}

	std::size_t serialization_identity = 11;

	void save(std::ostream &lbfile) {
		serialize(lbfile, initiative);
		for (const bool &b : permitted_work) {
			serialize(lbfile, b);
		}
	}

	static settler_ai_t load(std::istream &lbfile) {
		settler_ai_t c;
		deserialize(lbfile, c.initiative);
		for (std::size_t i=0; i<NUMBER_OF_JOB_CATEGORIES; ++i)
			deserialize(lbfile, c.permitted_work[i]);
		return c;
	}
};
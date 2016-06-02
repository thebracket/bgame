#pragma once

#include <rltk.hpp>
#include <array>
#include "position.hpp"

using namespace rltk;

constexpr int NUMBER_OF_JOB_CATEGORIES = 1;
constexpr int JOB_MINING = 0;

enum job_major_t { JOB_IDLE, JOB_SLEEP };
enum job_minor_t { 
	JM_NONE,
	JM_FIND_BED, JM_GO_TO_BED, JM_SLEEP			// Bed-time steps
};

struct settler_ai_t {

	int initiative = 0;
	std::array<bool, NUMBER_OF_JOB_CATEGORIES> permitted_work;
	int shift_id = 0;

	job_major_t job_type_major = JOB_IDLE;
	std::string job_status = "";
	job_minor_t job_type_minor = JM_NONE;
	int target_x = 0;
	int target_y = 0;
	int target_z = 0;

	// Non-persistent
	std::shared_ptr<rltk::navigation_path<position_t>> current_path;

	settler_ai_t() {
		std::fill(permitted_work.begin(), permitted_work.end(), true);
	}

	std::size_t serialization_identity = 11;

	void save(std::ostream &lbfile) {
		serialize(lbfile, initiative);
		for (const bool &b : permitted_work) {
			serialize(lbfile, b);
		}
		serialize(lbfile, shift_id);
		serialize(lbfile, job_type_major);
		serialize(lbfile, job_status);
		serialize(lbfile, job_type_minor);
		serialize(lbfile, target_x);
		serialize(lbfile, target_y);
		serialize(lbfile, target_z);
	}

	static settler_ai_t load(std::istream &lbfile) {
		settler_ai_t c;
		deserialize(lbfile, c.initiative);
		for (std::size_t i=0; i<NUMBER_OF_JOB_CATEGORIES; ++i) {
			deserialize(lbfile, c.permitted_work[i]);
		}
		deserialize(lbfile, c.shift_id);
		deserialize(lbfile, c.job_type_major);
		deserialize(lbfile, c.job_status);
		deserialize(lbfile, c.job_type_minor);
		deserialize(lbfile, c.target_x);
		deserialize(lbfile, c.target_y);
		deserialize(lbfile, c.target_z);

		return c;
	}
};
#pragma once

#include <rltk.hpp>
#include <array>
#include <boost/optional.hpp>
#include "position.hpp"
#include "designations.hpp"

using namespace rltk;

constexpr int NUMBER_OF_JOB_CATEGORIES = 3;
constexpr int JOB_MINING = 0;
constexpr int JOB_CHOPPING = 1;
constexpr int JOB_CONSTRUCTION = 2;

enum job_major_t { JOB_IDLE, JOB_SLEEP, JOB_MINE, JOB_CHOP, JOB_CONST };
enum job_minor_t { 
	JM_NONE,
	JM_FIND_BED, JM_GO_TO_BED, JM_SLEEP,				// Bed-time steps
	JM_FIND_PICK, JM_GO_TO_PICK, JM_COLLECT_PICK, JM_GO_TO_SITE, JM_DIG, JM_DROP_PICK,	// Mining steps
	JM_FIND_AXE, JM_GO_TO_AXE, JM_COLLECT_AXE, JM_FIND_TREE, JM_GO_TO_TREE, JM_CHOP, JM_DROP_AXE,	// Tree cutting steps
	JM_SELECT_COMPONENT, JM_GO_TO_COMPONENT, JM_COLLECT_COMPONENT, JM_GO_TO_BUILDING, JM_DROP_COMPONENT, JM_ASSEMBLE // Buildings
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
	int target_id = 0;
	boost::optional<building_designation_t> building_target;

	// Non-persistent
	std::shared_ptr<rltk::navigation_path<position_t>> current_path;
	std::size_t current_tool;

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
		serialize(lbfile, target_id);

		bool has_building_target = false;
		if (has_building_target) {
			has_building_target = true;
			serialize(lbfile, has_building_target);

			serialize(lbfile, building_target.get().x);
			serialize(lbfile, building_target.get().y);
			serialize(lbfile, building_target.get().z);
			serialize(lbfile, building_target.get().name);
			serialize(lbfile, building_target.get().tag);
			serialize(lbfile, building_target.get().width);
			serialize(lbfile, building_target.get().height);
			std::size_t sz = building_target.get().component_ids.size();
			serialize(lbfile, sz);
			for (const std::pair<std::size_t,bool> &id : building_target.get().component_ids) {
				serialize(lbfile, id.first);
				serialize(lbfile, id.second);
			}
			sz = building_target.get().components.size();
			serialize(lbfile, sz);
			for (const std::string &comp : building_target.get().components) {
				serialize(lbfile, comp);
			}
			sz = building_target.get().glyphs.size();
			serialize(lbfile, sz);
			for (const vchar &ch : building_target.get().glyphs) {
				serialize(lbfile, ch);
			}
			serialize(lbfile, building_target.get().building_entity);
		} else {
			serialize(lbfile, has_building_target);
		}
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
		deserialize(lbfile, c.target_id);
		bool has_building_target;
		deserialize(lbfile, has_building_target);
		if (has_building_target) {
			building_designation_t b;
			deserialize(lbfile, b.x);
			deserialize(lbfile, b.y);
			deserialize(lbfile, b.z);
			deserialize(lbfile, b.name);
			deserialize(lbfile, b.tag);
			deserialize(lbfile, b.width);
			deserialize(lbfile, b.height);

			std::size_t sz;
			deserialize(lbfile, sz);
			for (std::size_t i=0; i<sz; ++i) {
				std::pair<std::size_t,bool> comp;
				deserialize(lbfile, comp.first);
				deserialize(lbfile, comp.second);
				b.component_ids.push_back(comp);
			}
			deserialize(lbfile, sz);
			for (std::size_t i=0; i<sz; ++i) {
				std::string comp;
				deserialize(lbfile, comp);
				b.components.push_back(comp);
			}
			deserialize(lbfile, sz);
			for (std::size_t i=0; i<sz; ++i) {
				vchar g;
				deserialize(lbfile, g);
				b.glyphs.push_back(g);
			}
			deserialize(lbfile, c.building_target.get().building_entity);
			c.building_target = b;
		}

		return c;
	}
};
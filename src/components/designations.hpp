#pragma once

#include <rltk.hpp>
#include <boost/container/flat_map.hpp>
#include "../planet/region.hpp"
#include "position.hpp"
#include "../raws/reaction_input.hpp"

using namespace rltk;

struct building_designation_t {
	int x,y,z;
	std::vector<std::pair<std::size_t, bool>> component_ids;

	std::string name;
	std::string tag;
	std::vector<reaction_input_t> components;
    int width;
    int height;
    std::vector<rltk::vchar> glyphs;
	std::size_t building_entity = 0;
};

struct reaction_task_t {
    reaction_task_t() {}
    reaction_task_t(const std::size_t id, const std::string name, const std::string tag, std::vector<std::pair<std::size_t,bool>> comps) : building_id(id), job_name(name), reaction_tag(tag), components(comps) {}

    std::size_t building_id;
    std::string job_name;
	std::string reaction_tag;
    std::vector<std::pair<std::size_t,bool>> components;
};

// Standing orders
namespace standing_orders {

constexpr uint8_t SO_IDLE_STATIC = 0;
constexpr uint8_t SO_IDLE_WANDER = 1;
constexpr uint8_t SO_IDLE_WANDER_CLOSE = 2;

constexpr uint8_t SO_WILDLIFE_IGNORE = 0;
constexpr uint8_t SO_WILDLIFE_KILL = 1;

constexpr uint8_t SO_UPGRADE_NEVER = 0;
constexpr uint8_t SO_UPGRADE_NEARBY = 1;
constexpr uint8_t SO_UPGRADE_ALWAYS = 2;

}

struct unbuild_t {
	unbuild_t() {}
	unbuild_t(const bool &build, const std::size_t id) : is_building(build), building_id(id) {}
	bool is_building = true;
	std::size_t building_id;
};

struct designations_t {

	boost::container::flat_map<int, uint8_t> mining;
	boost::container::flat_map<int, position_t> chopping;
	std::vector<building_designation_t> buildings;
	std::vector<std::pair<uint8_t, std::string>> build_orders;
	std::vector<std::pair<bool, position_t>> guard_points;
	std::vector<unbuild_t> deconstructions;
	int current_power = 10;
	uint64_t current_cash = 100;

	uint8_t standing_order_idle_move = 1;
	uint8_t standing_order_wildlife_treatment = 1;
	uint8_t standing_order_upgrade = 1;

	// Not serialized
	rltk::color_t alert_color = rltk::colors::WHITE;
	int total_capacity = 10;

	designations_t() {
	}

	std::size_t serialization_identity = 13;

	void save(std::ostream &lbfile) {
		std::size_t size = mining.size();
		serialize(lbfile, size);

		for (auto it = mining.begin(); it != mining.end(); ++it) {
			serialize(lbfile, it->first);
			serialize(lbfile, it->second);
		}

		size = chopping.size();
		serialize(lbfile, size);

		for (auto it = chopping.begin(); it != chopping.end(); ++it) {
			serialize(lbfile, it->first);
			serialize(lbfile, it->second.x);
			serialize(lbfile, it->second.y);
			serialize(lbfile, it->second.z);
		}

		size = buildings.size();
		serialize(lbfile, size);
		for (auto it = buildings.begin(); it != buildings.end(); ++it) {
			serialize(lbfile, it->x);
			serialize(lbfile, it->y);
			serialize(lbfile, it->z);
			serialize(lbfile, it->name);
			serialize(lbfile, it->tag);
			serialize(lbfile, it->width);
			serialize(lbfile, it->height);
			std::size_t sz = it->component_ids.size();
			serialize(lbfile, sz);
			for (const std::pair<std::size_t,bool> &id : it->component_ids) {
				serialize(lbfile, id.first);
				serialize(lbfile, id.second);
			}
			sz = it->components.size();
			serialize(lbfile, sz);
			for (const reaction_input_t &comp : it->components) {
				serialize(lbfile, comp.tag);
				serialize(lbfile, comp.quantity);
				bool has_material = true; 
				if (!comp.required_material) has_material = false;
				serialize(lbfile, has_material);				
				if (has_material) serialize(lbfile, comp.required_material.get());

				bool has_mat_type = true;
				if (!comp.required_material_type) has_mat_type = false;
				serialize(lbfile, has_mat_type);
				if (has_mat_type) serialize(lbfile, comp.required_material_type.get());
			}
			sz = it->glyphs.size();
			serialize(lbfile, sz);
			for (const vchar &ch : it->glyphs) {
				serialize(lbfile, ch);
			}
			serialize(lbfile, it->building_entity);
		}
		size = build_orders.size();
		serialize(lbfile, size);
		for (const auto &order : build_orders) {
			serialize(lbfile, order.first);
			serialize(lbfile, order.second);
		}

		size = guard_points.size();
		serialize(lbfile, size);
		for (const auto &g : guard_points) {
			serialize(lbfile, g.first);
			serialize(lbfile, g.second);
		}

		size = deconstructions.size();
		serialize(lbfile, size);
		for (const auto &u : deconstructions) {
			serialize(lbfile, u.is_building);
			serialize(lbfile, u.building_id);
		}

		serialize(lbfile, current_power);
		serialize(lbfile, current_cash);
		serialize(lbfile, standing_order_idle_move);
		serialize(lbfile, standing_order_wildlife_treatment);
		serialize(lbfile, standing_order_upgrade);
	}

	static designations_t load(std::istream &lbfile) {
		designations_t c;
		std::size_t size;
		deserialize(lbfile, size);
		for (std::size_t i=0; i<size; ++i) {
			int idx;
			uint8_t tmp;
			deserialize(lbfile, idx);
			deserialize(lbfile, tmp);
			c.mining[idx] = tmp;
		}

		deserialize(lbfile, size);
		for (std::size_t i=0; i<size; ++i) {
			int tree_id;
			int x,y,z;
			deserialize(lbfile, tree_id);
			deserialize(lbfile, x);
			deserialize(lbfile, y);
			deserialize(lbfile, z);
			c.chopping[tree_id] = position_t{x,y,z};
		}

		deserialize(lbfile, size);
		for (std::size_t i=0; i<size; ++i) {
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

				reaction_input_t comp;
				deserialize(lbfile, comp.tag);
				deserialize(lbfile, comp.quantity);
				bool has_material;
				deserialize(lbfile, has_material);
				if (has_material) {
					std::size_t mat;
					deserialize(lbfile, mat);
					comp.required_material = mat;
				}
				bool has_mat_type;
				deserialize(lbfile, has_mat_type);
				if (has_mat_type) {
					material_def_spawn_type_t tmp;
					deserialize(lbfile, tmp);
					comp.required_material_type = tmp;
				}

				b.components.push_back(comp);
			}
			deserialize(lbfile, sz);
			for (std::size_t i=0; i<sz; ++i) {
				vchar g;
				deserialize(lbfile, g);
				b.glyphs.push_back(g);
			}

			deserialize(lbfile, b.building_entity);
			c.buildings.push_back(b);
		}

		std::size_t sz;
		deserialize(lbfile, sz);
		for (std::size_t i=0; i<sz; ++i) {
			uint8_t build_qty;
			std::string build_name;
			deserialize(lbfile, build_qty);
			deserialize(lbfile, build_name);
			c.build_orders.push_back(std::make_pair(build_qty, build_name));
		}

		deserialize(lbfile, sz);
		for (std::size_t i=0; i<sz; ++i) {
			bool claimed;
			position_t pos;
			deserialize(lbfile, claimed);
			deserialize(lbfile, pos);
			c.guard_points.push_back(std::make_pair(claimed, pos));
		}

		deserialize(lbfile, sz);
		for (std::size_t i=0; i<sz; ++i) {
			unbuild_t u;
			deserialize(lbfile, u.is_building);
			deserialize(lbfile, u.building_id);
			c.deconstructions.push_back(u);
		}

		deserialize(lbfile, c.current_power);
		deserialize(lbfile, c.current_cash);
		deserialize(lbfile, c.standing_order_idle_move);
		deserialize(lbfile, c.standing_order_wildlife_treatment);
		deserialize(lbfile, c.standing_order_upgrade);
		return c;
	}
};
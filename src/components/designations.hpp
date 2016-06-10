#pragma once

#include <rltk.hpp>
#include <boost/container/flat_map.hpp>
#include "../planet/region.hpp"
#include "position.hpp"

using namespace rltk;

struct building_designation_t {
	int x,y,z;
	std::vector<std::pair<std::size_t, bool>> component_ids;

	std::string name;
	std::string tag;
	std::vector<std::string> components;
    int width;
    int height;
    std::vector<rltk::vchar> glyphs;
	std::size_t building_entity = 0;
};

struct designations_t {

	boost::container::flat_map<int, uint8_t> mining;
	boost::container::flat_map<int, position_t> chopping;
	std::vector<building_designation_t> buildings;

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
			for (const std::string &comp : it->components) {
				serialize(lbfile, comp);
			}
			sz = it->glyphs.size();
			serialize(lbfile, sz);
			for (const vchar &ch : it->glyphs) {
				serialize(lbfile, ch);
			}
			serialize(lbfile, it->building_entity);
		}
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

			deserialize(lbfile, b.building_entity);
			c.buildings.push_back(b);
		}

		return c;
	}
};
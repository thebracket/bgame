#include "geometry_helper.hpp"
#include "../../planet/region/region.hpp"
#include "../../raws/materials.hpp"
#include "../../raws/defs/material_def_t.hpp"

namespace chunks {
	unsigned int get_floor_tex(const int &idx) {
		if (region::veg_type(idx) > 0) return 0; // Grass is determined to be index 0
		auto material_idx = region::material(idx);
		auto material = get_material(material_idx);
		if (!material) return 3;

		unsigned int use_id = 3;
		if (region::flag(idx, CONSTRUCTION)) {
			use_id = (unsigned int)material->constructed_floor_texture_id;
		}
		else {
			use_id = (unsigned int)material->floor_texture_id;
		}
		if (use_id == 3) std::cout << "Material [" << material->name << "] is lacking a texture\n";

		//std::cout << material->name << "=" << use_id << "\n";
		return use_id;
	}

	unsigned int get_cube_tex(const int &idx) {
		auto tt = region::tile_type(idx);
		if (tt == tile_type::TREE_TRUNK) return 6;
		if (tt == tile_type::TREE_LEAF) return 9;

		auto material_idx = region::material(idx);
		auto material = get_material(material_idx);
		if (!material) return 3;

		unsigned int use_id = 3;
		if (region::flag(idx, CONSTRUCTION)) {
			use_id = (unsigned int)material->constructed_texture_id;
		}
		else {
			use_id = (unsigned int)material->base_texture_id;
		}
		if (use_id == 3) std::cout << "Material [" << material->name << "] is lacking a texture\n";
		return use_id;
	}

	bool is_cube(const uint8_t type) {
		switch (type) {
		case tile_type::SOLID: return true;
		case tile_type::WALL: return true;
		case tile_type::TREE_LEAF: return true;
		case tile_type::TREE_TRUNK: return true;
		case tile_type::SEMI_MOLTEN_ROCK: return true;
		case tile_type::CLOSED_DOOR: return true;
		default: return false;
		}
	}

	int add_floor_geometry(std::vector<float> &v, const float &x, const float &y, const float &z,
		const float &width, const float &height, const float &texture_id)
	{
		const float x0 = -0.5f + x;
		const float x1 = x0 + width;
		const float y0 = -0.5f + z;
		//const float y1 = y0 + 1.0f; // We don't use y1 for floors
		const float z0 = -0.5f + y;
		const float z1 = z0 + height;
		const float TI = texture_id;
		constexpr float T0 = 0.0f;
		const float TW = width;
		const float TH = height;
		constexpr float ceiling_gap = 0.001f;

		v.insert(v.end(), {
			// Upwards facing floor
			x1, y0, z1, TW, TH, TI,  0.0f,  1.0f,  0.0f,
			x1, y0, z0, TW, T0, TI,  0.0f,  1.0f,  0.0f,
			x0, y0, z0, T0, T0, TI,  0.0f,  1.0f,  0.0f,
			x0, y0, z0, T0, T0, TI,  0.0f,  1.0f,  0.0f,
			x0, y0, z1, T0, TH, TI,  0.0f,  1.0f,  0.0f,
			x1, y0, z1, TW, TH, TI,  0.0f,  1.0f,  0.0f
		});
		return 6;
	}

	int add_cube_geometry(std::vector<float> &v, const float &x, const float &y, const float &z,
		const float &width, const float &height, const float &texture_id, float depth)
	{
		const float x0 = -0.5f + x;
		const float x1 = x0 + width;
		const float y0 = -0.5f + z;
		const float y1 = y0 + depth;
		const float z0 = -0.5f + y;
		const float z1 = z0 + height;
		const float TI = texture_id;
		constexpr float T0 = 0.0f;
		const float TW = width;
		const float TH = height;

		v.insert(v.end(), {
			// Back side
			x0, y0, z0, T0, T0, TI,  0.0f,  0.0f, -1.0f,
			x1, y0, z0, TW, T0, TI,  0.0f,  0.0f, -1.0f,
			x1, y1, z0, TW, TH, TI,  0.0f,  0.0f, -1.0f,
			x1, y1, z0, TW, TH, TI,  0.0f,  0.0f, -1.0f,
			x0, y1, z0, T0, TH, TI,  0.0f,  0.0f, -1.0f,
			x0, y0, z0, T0, TH, TI,  0.0f,  0.0f, -1.0f,

			// Front side
			x0, y0, z1, T0, T0, TI,  0.0f,  0.0f, 1.0f,
			x1, y0, z1, TW, T0, TI,  0.0f,  0.0f, 1.0f,
			x1, y1, z1, TW, TH, TI,  0.0f,  0.0f, 1.0f,
			x1, y1, z1, TW, TH, TI,  0.0f,  0.0f, 1.0f,
			x0, y1, z1, T0, TH, TI,  0.0f,  0.0f, 1.0f,
			x0, y0, z1, T0, T0, TI,  0.0f,  0.0f, 1.0f,

			// Left side
			x0, y1, z1, TW, TH, TI, -1.0f,  0.0f,  0.0f,
			x0, y1, z0, TW, T0, TI, -1.0f,  0.0f,  0.0f,
			x0, y0, z0, T0, T0, TI, -1.0f,  0.0f,  0.0f,
			x0, y0, z0, T0, T0, TI, -1.0f,  0.0f,  0.0f,
			x0, y0, z1, T0, TH, TI, -1.0f,  0.0f,  0.0f,
			x0, y1, z1, TW, TH, TI, -1.0f,  0.0f,  0.0f,

			// Right side
			x1, y1, z1, TW, TH, TI,  1.0f,  0.0f,  0.0f,
			x1, y1, z0, TW, T0, TI,  1.0f,  0.0f,  0.0f,
			x1, y0, z0, T0, T0, TI,  1.0f,  0.0f,  0.0f,
			x1, y0, z0, T0, T0, TI,  1.0f,  0.0f,  0.0f,
			x1, y0, z1, T0, TH, TI,  1.0f,  0.0f,  0.0f,
			x1, y1, z1, TW, TH, TI,  1.0f,  0.0f,  0.0f,

			// Underside
			x0, y0, z0, T0, T0, TI,  0.0f, -1.0f,  0.0f,
			x1, y0, z0, TW, T0, TI,  0.0f, -1.0f,  0.0f,
			x1, y0, z1, TW, TH, TI,  0.0f, -1.0f,  0.0f,
			x1, y0, z1, TW, TH, TI,  0.0f, -1.0f,  0.0f,
			x0, y0, z1, T0, TH, TI,  0.0f, -1.0f,  0.0f,
			x0, y0, z0, T0, T0, TI,  0.0f, -1.0f,  0.0f,

			// Top
			x0, y1, z0, T0, T0, TI,  0.0f,  1.0f,  0.0f,
			x1, y1, z0, TW, T0, TI,  0.0f,  1.0f,  0.0f,
			x1, y1, z1, TW, TH, TI,  0.0f,  1.0f,  0.0f,
			x1, y1, z1, TW, TH, TI,  0.0f,  1.0f,  0.0f,
			x0, y1, z1, T0, TH, TI,  0.0f,  1.0f,  0.0f,
			x0, y1, z0, T0, T0, TI,  0.0f,  1.0f,  0.0f
		});
		return 36;
	}

	int add_water_geometry(std::vector<float> &v, const float &x, const float &y, const float &z,
		const float &width, const float &height, const float &texture_id, const uint8_t &water_level)
	{
		const float water_y = static_cast<float>(water_level) / 10.0f;
		const float x0 = -0.5f + x;
		const float x1 = x0 + width;
		const float y0 = -0.5f + z + water_y;
		//const float y1 = y0 + 1.0f; // We don't use y1 for floors
		const float z0 = -0.5f + y + water_y;
		const float z1 = z0 + height;
		const float TI = texture_id;
		constexpr float T0 = 0.0f;
		const float TW = width;
		const float TH = height;
		constexpr float ceiling_gap = 0.001f;

		v.insert(v.end(), {
			// Upwards facing floor
			x1, y0, z1, TW, TH, TI,  0.0f,  1.0f,  0.0f,
			x1, y0, z0, TW, T0, TI,  0.0f,  1.0f,  0.0f,
			x0, y0, z0, T0, T0, TI,  0.0f,  1.0f,  0.0f,
			x0, y0, z0, T0, T0, TI,  0.0f,  1.0f,  0.0f,
			x0, y0, z1, T0, TH, TI,  0.0f,  1.0f,  0.0f,
			x1, y0, z1, TW, TH, TI,  0.0f,  1.0f,  0.0f,
		});
		return 6;
	}

	int add_ramp_geometry(std::vector<float> &v, const float &x, const float &y, const float &z,
		const float &width, const float &height, const float &texture_id, const float &ne, const float &se, const float &sw, const float &nw)
	{
		const float x0 = -0.5f + x;
		const float x1 = x0 + width;
		const float y0 = -0.5f + z;
		//const float y1 = y0 + 1.0f; // We don't use y1 for floors
		const float z0 = -0.5f + y;
		const float z1 = z0 + height;
		const float TI = texture_id;
		constexpr float T0 = 0.0f;
		const float TW = width;
		const float TH = height;
		constexpr float ceiling_gap = 0.001f;

		v.insert(v.end(), {
			// Upwards facing floor
			x1, y0 + ne, z1, TW, TH, TI,  0.0f,  1.0f,  0.0f, // NE
			x1, y0 + se, z0, TW, T0, TI,  0.0f,  1.0f,  0.0f, // SE
			x0, y0 + sw, z0, T0, T0, TI,  0.0f,  1.0f,  0.0f, // SW
			x0, y0 + sw, z0, T0, T0, TI,  0.0f,  1.0f,  0.0f, // SW
			x0, y0 + nw, z1, T0, TH, TI,  0.0f,  1.0f,  0.0f, // NW
			x1, y0 + ne, z1, TW, TH, TI,  0.0f,  1.0f,  0.0f, // NE
		});
		return 6;
	}
}

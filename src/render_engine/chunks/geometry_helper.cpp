#include "geometry_helper.hpp"
#include "../../planet/region/region.hpp"
#include "../../raws/materials.hpp"
#include "../../raws/defs/material_def_t.hpp"
#include <glm/glm.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace tile_flags;

namespace chunks {

	constexpr float NORMAL_UP = 0.0f;
	constexpr float NORMAL_BACK = 1.0f;
	constexpr float NORMAL_FRONT = 2.0f;
	constexpr float NORMAL_LEFT = 3.0f;
	constexpr float NORMAL_RIGHT = 4.0f;
	constexpr float NORMAL_BOTTOM = 5.0f;

	unsigned int get_floor_tex(const int &idx) {
		// If its a stockpile, render it as such
		if (region::stockpile_id(idx) > 0) return 3; // TODO: Determine texture

		// We no longer hard-code grass.
		if (region::veg_type(idx) > 0 && !region::flag(idx, CONSTRUCTION)) {
			switch (region::veg_lifecycle(idx)) {
			case 0: return 18; // Germination
			case 1: return 21; // Sprouting
			case 2: return 0; // Growing (grass is material 0)
			case 3: return 24; // Flowering
			}
			return 0; // Grass is determined to be index 0
		}
		const auto material_idx = region::material(idx);
		const auto material = get_material(material_idx);
		if (!material) return 3;

		unsigned int use_id = 3;
		if (region::flag(idx, CONSTRUCTION)) {
			use_id = (unsigned int)material->constructed_floor_texture_id;
		}
		else {
			use_id = (unsigned int)material->floor_texture_id;
		}
		if (use_id == 3) {
			glog(log_target::LOADER, log_severity::warning, "Material [{0}] is lacking a texture", material->name);
		}

		//std::cout << material->name << "=" << use_id << "\n";
		return use_id;
	}

	unsigned int get_cube_tex(const int &idx) {
		const auto tt = region::tile_type(idx);
		if (tt == tile_type::TREE_TRUNK) return 6;
		if (tt == tile_type::TREE_LEAF) return 9;

		const auto material_idx = region::material(idx);
		const auto material = get_material(material_idx);
		if (!material) return 3;

		unsigned int use_id = 3;
		if (!region::flag(idx, CONSTRUCTION)) {
			use_id = (unsigned int)material->constructed_texture_id;
		}
		else {
			use_id = (unsigned int)material->base_texture_id;
		}
		if (use_id == 3) {
			glog(log_target::LOADER, log_severity::warning, "Material [{0}] is lacking a texture.", material->name);
		}
		return use_id;
	}

	bool is_cube(const uint8_t type) {
		switch (type) {
		case tile_type::SOLID: return true;
		case tile_type::WALL: return true;
		case tile_type::TREE_LEAF: return true;
		case tile_type::TREE_TRUNK: return true;
		case tile_type::SEMI_MOLTEN_ROCK: return true;
		default: return false;
		}
	}

	static std::pair<glm::vec3, glm::vec3> calculate_tangent(
		const float x0, const float y0, const float z0, const float tx0, const float ty0,
		const float x1, const float y1, const float z1, const float tx1, const float ty1,
		const float x2, const float y2, const float z2, const float tx2, const float ty2,
		const float normal_x, const float normal_y, const float normal_z
	)
	{
		const glm::vec3 pos1{ x0, y0, z0 };
		const glm::vec3 pos2{ x1, y1, z1 };
		const glm::vec3 pos3{ x2, y2, z2 };
		const glm::vec2 uv1{ tx0, ty0 };
		const glm::vec2 uv2{ tx1, ty1 };
		const glm::vec2 uv3{ tx2, ty2 };
		const glm::vec3 nm{ normal_x, normal_y, normal_z };
		const glm::vec3 edge1 = pos2 - pos1;
		const glm::vec3 edge2 = pos3 - pos1;
		const glm::vec2 deltaUV1 = uv2 - uv1;
		const glm::vec2 deltaUV2 = uv3 - uv1;

		const float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		glm::vec3 tangent1;
		glm::vec3 bitangent1;
		tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent1 = glm::normalize(tangent1);

		bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent1 = glm::normalize(bitangent1);

		return std::make_pair(tangent1, bitangent1);
	}

	struct tmp_vec_3 { float x; float y; float z; };
	constexpr tmp_vec_3 FLOOR_TANGENT{ 1.0f, 0.0f, 0.0f };
	constexpr tmp_vec_3 WALL_TANGENT{ 0.0f, 1.0f, 0.0f };

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

		/*
		const auto tangent1 = calculate_tangent(
			x1, y0, z1, TW, TH,
			x1, y0, z0, TW, T0,
			x0, y0, z0, T0, T0,
			0.0f, 1.0f, 0.0f
		);
		std::cout << "Tangent for a floor is equal to: " << tangent1.x << ", " << tangent1.y << ", " << tangent1.z << "\n";
		*/

		v.insert(v.end(), {
			// Upwards facing floor
			x1, y0, z1, z, TW, TH, TI,  NORMAL_UP,
			x1, y0, z0, z, TW, T0, TI,  NORMAL_UP,
			x0, y0, z0, z, T0, T0, TI,  NORMAL_UP,
			x0, y0, z0, z, T0, T0, TI,  NORMAL_UP,
			x0, y0, z1, z, T0, TH, TI,  NORMAL_UP,
			x1, y0, z1, z, TW, TH, TI,  NORMAL_UP,
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
			x0, y0, z0, z, T0, T0, TI,  NORMAL_BACK,
			x1, y1, z0, z, TW, TH, TI,  NORMAL_BACK,
			x1, y0, z0, z, TW, T0, TI,  NORMAL_BACK,
			x1, y1, z0, z, TW, TH, TI,  NORMAL_BACK,
			x0, y0, z0, z, T0, T0, TI,  NORMAL_BACK,
			x0, y1, z0, z, T0, TH, TI,  NORMAL_BACK,

			x0, y0, z1, z, T0, T0, TI,  NORMAL_FRONT,
			x1, y0, z1, z, TW, T0, TI,  NORMAL_FRONT,
			x1, y1, z1, z, TW, TH, TI,  NORMAL_FRONT,
			x1, y1, z1, z, TW, TH, TI,  NORMAL_FRONT,
			x0, y1, z1, z, T0, TH, TI,  NORMAL_FRONT,
			x0, y0, z1, z, T0, T0, TI,  NORMAL_FRONT,

			x0, y1, z1, z, TW, TH, TI, NORMAL_LEFT,
			x0, y1, z0, z, TW, T0, TI, NORMAL_LEFT,
			x0, y0, z0, z, T0, T0, TI, NORMAL_LEFT,
			x0, y0, z0, z, T0, T0, TI, NORMAL_LEFT,
			x0, y0, z1, z, T0, TH, TI, NORMAL_LEFT,
			x0, y1, z1, z, TW, TH, TI, NORMAL_LEFT,

			x1, y1, z1, z, TW, TH, TI,  NORMAL_RIGHT,
			x1, y0, z0, z, T0, T0, TI,  NORMAL_RIGHT,
			x1, y1, z0, z, TW, T0, TI,  NORMAL_RIGHT,
			x1, y0, z0, z, T0, T0, TI,  NORMAL_RIGHT,
			x1, y1, z1, z, TW, TH, TI,  NORMAL_RIGHT,
			x1, y0, z1, z, T0, TH, TI,  NORMAL_RIGHT,

			x0, y0, z0, z, TW, TH, TI,  NORMAL_BOTTOM,
			x1, y0, z0, z, TW, T0, TI,  NORMAL_BOTTOM,
			x1, y0, z1, z, T0, T0, TI,  NORMAL_BOTTOM,
			x1, y0, z1, z, T0, T0, TI,  NORMAL_BOTTOM,
			x0, y0, z1, z, T0, TH, TI,  NORMAL_BOTTOM,
			x0, y0, z0, z, TW, TH, TI,  NORMAL_BOTTOM,

			x1, y1, z1, z, TW, TH, TI,  NORMAL_UP,
			x1, y1, z0, z, TW, T0, TI,  NORMAL_UP,
			x0, y1, z0, z, T0, T0, TI,  NORMAL_UP,
			x0, y1, z0, z, T0, T0, TI,  NORMAL_UP,
			x0, y1, z1, z, T0, TH, TI,  NORMAL_UP,
			x1, y1, z1, z, TW, TH, TI,  NORMAL_UP,
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
		const float z0 = -0.5f + y;
		const float z1 = z0 + height;
		const float TI = texture_id;
		constexpr float T0 = 0.0f;
		const float TW = width;
		const float TH = height;
		constexpr float ceiling_gap = 0.001f;

		v.insert(v.end(), {
			// Upwards facing floor
			x1, y0, z1, z, TW, TH, TI,  NORMAL_UP,
			x1, y0, z0, z, TW, T0, TI,  NORMAL_UP,
			x0, y0, z0, z, T0, T0, TI,  NORMAL_UP,
			x0, y0, z0, z, T0, T0, TI,  NORMAL_UP,
			x0, y0, z1, z, T0, TH, TI,  NORMAL_UP,
			x1, y0, z1, z, TW, TH, TI,  NORMAL_UP,
		});
		return 6;
	}

	int add_ramp_geometry(std::vector<float> &v, const float &x, const float &y, const float &z,
		const float &width, const float &height, const float &texture_id, const float &ne, const float &se, const float &sw, const float &nw)
	{
		const float x0 = -0.5f + x;
		const float x1 = x0 + width;
		const float y0 = -0.5f + z;
		const float y1 = y0 + 0.5f; // We don't use y1 for floors
		const float z0 = -0.5f + y;
		const float z1 = z0 + height;
		const float TI = texture_id;
		constexpr float T0 = 0.0f;
		const float TW = width;
		const float TH = height;
		constexpr float ceiling_gap = 0.001f;
		
		/*
		v.insert(v.end(), {
			// Upwards facing floor
			x1, y0 + ne, z1, z, TW, TH, TI,  NORMAL_UP,
			x1, y0 + se, z0, z, TW, T0, TI,  NORMAL_UP,
			x0, y0 + sw, z0, z, T0, T0, TI,  NORMAL_UP,
			x0, y0 + sw, z0, z, T0, T0, TI,  NORMAL_UP,
			x0, y0 + nw, z1, z, T0, TH, TI,  NORMAL_UP,
			x1, y0 + ne, z1, z, TW, TH, TI,  NORMAL_UP,
		});
		return 6;
		*/

		v.insert(v.end(), {
			x0, y0, z0, z, T0, T0, TI,  NORMAL_BACK,
			x1, y1, z0, z, TW, TH, TI,  NORMAL_BACK,
			x1, y0, z0, z, TW, T0, TI,  NORMAL_BACK,
			x1, y1, z0, z, TW, TH, TI,  NORMAL_BACK,
			x0, y0, z0, z, T0, T0, TI,  NORMAL_BACK,
			x0, y1, z0, z, T0, TH, TI,  NORMAL_BACK,

			x0, y0, z1, z, T0, T0, TI,  NORMAL_FRONT,
			x1, y0, z1, z, TW, T0, TI,  NORMAL_FRONT,
			x1, y1, z1, z, TW, TH, TI,  NORMAL_FRONT,
			x1, y1, z1, z, TW, TH, TI,  NORMAL_FRONT,
			x0, y1, z1, z, T0, TH, TI,  NORMAL_FRONT,
			x0, y0, z1, z, T0, T0, TI,  NORMAL_FRONT,

			x0, y1, z1, z, TW, TH, TI, NORMAL_LEFT,
			x0, y1, z0, z, TW, T0, TI, NORMAL_LEFT,
			x0, y0, z0, z, T0, T0, TI, NORMAL_LEFT,
			x0, y0, z0, z, T0, T0, TI, NORMAL_LEFT,
			x0, y0, z1, z, T0, TH, TI, NORMAL_LEFT,
			x0, y1, z1, z, TW, TH, TI, NORMAL_LEFT,

			x1, y1, z1, z, TW, TH, TI,  NORMAL_RIGHT,
			x1, y0, z0, z, T0, T0, TI,  NORMAL_RIGHT,
			x1, y1, z0, z, TW, T0, TI,  NORMAL_RIGHT,
			x1, y0, z0, z, T0, T0, TI,  NORMAL_RIGHT,
			x1, y1, z1, z, TW, TH, TI,  NORMAL_RIGHT,
			x1, y0, z1, z, T0, TH, TI,  NORMAL_RIGHT,

			x0, y0, z0, z, TW, TH, TI,  NORMAL_BOTTOM,
			x1, y0, z0, z, TW, T0, TI,  NORMAL_BOTTOM,
			x1, y0, z1, z, T0, T0, TI,  NORMAL_BOTTOM,
			x1, y0, z1, z, T0, T0, TI,  NORMAL_BOTTOM,
			x0, y0, z1, z, T0, TH, TI,  NORMAL_BOTTOM,
			x0, y0, z0, z, TW, TH, TI,  NORMAL_BOTTOM,

			x1, y1, z1, z, TW, TH, TI,  NORMAL_UP,
			x1, y1, z0, z, TW, T0, TI,  NORMAL_UP,
			x0, y1, z0, z, T0, T0, TI,  NORMAL_UP,
			x0, y1, z0, z, T0, T0, TI,  NORMAL_UP,
			x0, y1, z1, z, T0, TH, TI,  NORMAL_UP,
			x1, y1, z1, z, TW, TH, TI,  NORMAL_UP,
			});
		return 36;
	}
}

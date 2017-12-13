#pragma once

namespace chunks {
	unsigned int get_floor_tex(const int &idx);
	unsigned int get_cube_tex(const int &idx);
	bool is_cube(const uint8_t type);

	int add_floor_geometry(std::vector<float> &v, const float &x, const float &y, const float &z,
		const float &width, const float &height, const float &texture_id);

	int add_cube_geometry(std::vector<float> &v, const float &x, const float &y, const float &z,
		const float &width, const float &height, const float &texture_id, float depth=0.98f);

	int add_water_geometry(std::vector<float> &v, const float &x, const float &y, const float &z,
		const float &width, const float &height, const float &texture_id, const uint8_t &water_level);

	int add_ramp_geometry(std::vector<float> &v, const float &x, const float &y, const float &z,
		const float &width, const float &height, const float &texture_id, const float &ne, const float &se, const float &sw, const float &nw);
}

#pragma once

struct world_position_t {
	int world_x=0;
	int world_y=0;
	int region_x=0;
	int region_y=0;
	int region_z=0;

	world_position_t(const int wx, const int wy, const int rx, const int ry, const int rz) noexcept :
		world_x(wx), world_y(wy), region_x(rx), region_y(ry), region_z(rz) {}
	world_position_t() = default;	
};


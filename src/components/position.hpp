#pragma once

struct position_t {
	int x=0;
	int y=0;
	int z=0;
    float offset_x = 0;
    float offset_y = 0;
    float offset_z = 0;
	int rotation = 0;

	position_t() = default;
	position_t(const int &nx, const int &ny, const int &nz) noexcept : x(nx), y(ny), z(nz) {}
    position_t(const float &nx, const float &ny, const int &nz) noexcept : x(static_cast<int>(nx)), y(static_cast<int>(ny)), z(nz) {}
	position_t(const int &nx, const int &ny, const int &nz, const int &rot) noexcept : x(nx), y(ny), z(nz), rotation(rot) {}
	position_t(const float &nx, const float &ny, const int &nz, const int &rot) noexcept : x(static_cast<int>(nx)), y(static_cast<int>(ny)), z(nz), rotation(rot) {}
	bool operator==(const position_t &rhs) const { return (x==rhs.x && y==rhs.y && z==rhs.z); }
};


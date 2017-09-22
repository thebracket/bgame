#pragma once

#include "../bengine/ecs.hpp"

struct position_t {
	int x=0;
	int y=0;
	int z=0;
    float offsetX = 0;
    float offsetY = 0;
    float offsetZ = 0;
	int rotation = 0;

	position_t() {}
	position_t(const int &X, const int &Y, const int &Z) : x(X), y(Y), z(Z) {}
    position_t(const float &X, const float &Y, const int &Z) : x(static_cast<int>(X)), y(static_cast<int>(Y)), z(Z) {}
	position_t(const int &X, const int &Y, const int &Z, const int &rot) : x(X), y(Y), z(Z), rotation(rot) {}
	position_t(const float &X, const float &Y, const int &Z, const int &rot) : x(static_cast<int>(X)), y(static_cast<int>(Y)), z(Z), rotation(rot) {}
	bool operator==(position_t &rhs) { return (x==rhs.x && y==rhs.y && z==rhs.z); }

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive( x, y, z, offsetX, offsetY, offsetZ, rotation ); // serialize things by passing them to the archive
	}
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<position_t>>)
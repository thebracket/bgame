#pragma once

#include <rltk.hpp>

using namespace rltk;

struct world_position_t {
	int world_x=0;
	int world_y=0;
	int region_x=0;
	int region_y=0;
	int region_z=0;
	std::string xml_identity = "world_position_t";

	world_position_t(const int wx, const int wy, const int rx, const int ry, const int rz) :
		world_x(wx), world_y(wy), region_x(rx), region_y(ry), region_z(rz) {}
	world_position_t() {}

	void to_xml(xml_node * c);
	void from_xml(xml_node * c);
};
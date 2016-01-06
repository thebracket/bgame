#pragma once

#include <fstream>
#include <string>
#include "../../engine/virtual_terminal.hpp"
#include "../world/location.hpp"

using std::ostream;
using std::istream;
using std::string;
using engine::vterm::color_t;

enum component_type
{
	dummy,
	name,
	renderable,
	viewshed,
	calendar,
	settler_ai,
	obstruction,
	power_generator,
	power_battery,
	gamestats,
	gamespecies,
	gamehealth,
	provision,
	item_storage,
	item_carried,
	description,
	item,
	tree,
	particle_emitter,
	position3d,
	walkable_roof,
	pointlight
};

namespace serialization_generic
{
/* Primitive save/load handler */

template<typename T>
inline void save_primitive(ostream &lbfile, const T &target)
{
	lbfile.write(reinterpret_cast<const char *>(&target), sizeof(target));
}
template<>
inline void save_primitive(ostream &lbfile, const string &s)
{
	unsigned int size = s.size();
	save_primitive<unsigned int>(lbfile, size);
	for (unsigned int i = 0; i < size; ++i)
	{
		save_primitive<char>(lbfile, s[i]);
	}
}
template<>
inline void save_primitive(ostream &lbfile, const color_t &c)
{
	unsigned char red = std::get<0>(c);
	unsigned char green = std::get<1>(c);
	unsigned char blue = std::get<2>(c);
	save_primitive<unsigned char>(lbfile, red);
	save_primitive<unsigned char>(lbfile, green);
	save_primitive<unsigned char>(lbfile, blue);
}
template<>
inline void save_primitive(ostream &lbfile, const bool &b)
{
	int n = 0;
	if (b)
		n = 1;
	save_primitive<int>(lbfile, n);
}
template<>
inline void save_primitive(ostream &lbfile, const component_type &t)
{
	int n = t;
	save_primitive<int>(lbfile, n);
}
template<>
inline void save_primitive(ostream &lbfile, const location_t &t)
{
	save_primitive<uint16_t>(lbfile, t.region);
	save_primitive<int16_t>(lbfile, t.x);
	save_primitive<int16_t>(lbfile, t.y);
	save_primitive<uint8_t>(lbfile, t.z);
}

template<typename T>
inline void load_primitive(istream &lbfile, T &target)
{
	lbfile.read(reinterpret_cast<char *>(&target), sizeof(target));
}
template<>
inline void load_primitive(istream &lbfile, string &target)
{
	string result;
	unsigned int size = 0;
	load_primitive<unsigned int>(lbfile, size);
	for (std::size_t i = 0; i < size; ++i)
	{
		char c;
		load_primitive<char>(lbfile, c);
		result += c;
	}
	target = result;
}
template<>
inline void load_primitive(istream &lbfile, color_t &c)
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	load_primitive<unsigned char>(lbfile, red);
	load_primitive<unsigned char>(lbfile, green);
	load_primitive<unsigned char>(lbfile, blue);
	c = color_t
	{ red, green, blue };
}
template<>
inline void load_primitive(istream &lbfile, bool &b)
{
	int n = 0;
	load_primitive<int>(lbfile, n);
	if (n == 0)
	{
		b = false;
	}
	else
	{
		b = true;
	}
}
template<>
inline void load_primitive(istream &lbfile, component_type &t)
{
	int n = 0;
	load_primitive<int>(lbfile, n);
	t = static_cast<component_type>(n);
}
template<>
inline void load_primitive(istream &lbfile, location_t &t)
{
	load_primitive<uint16_t>(lbfile, t.region);
	load_primitive<int16_t>(lbfile, t.x);
	load_primitive<int16_t>(lbfile, t.y);
	load_primitive<uint8_t>(lbfile, t.z);
}

/* Common Properties */

template<typename T>
inline void save_common_component_properties(ostream &lbfile, const T &target)
{
	save_primitive<component_type>(lbfile, target.type);
	save_primitive<int>(lbfile, target.entity_id);
}

template<typename T>
inline void load_common_component_properties(istream &lbfile, T &target)
{
	load_primitive<int>(lbfile, target.entity_id);
}

}

#pragma once

#include <rltk.hpp>

using namespace rltk;

struct construct_power_t {
	int storage_capacity;
	int generation_always;
	int generation_solar;

	construct_power_t() {}
	construct_power_t(const int store, const int gen_all, const int gen_solar) : storage_capacity(store), generation_always(gen_all), generation_solar(gen_solar) {}

	std::string xml_identity = "construct_power_t";

	void to_xml(xml_node * c);
	void from_xml(xml_node * c);
};
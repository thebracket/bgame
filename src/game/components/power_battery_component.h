#pragma once

#include "component_types.h"

using namespace serialization_generic;

struct power_battery_component
{
	int entity_id;
	int handle;
	component_type type = power_battery;
	bool deleted = false;

	power_battery_component()
	{
	}
	power_battery_component(const int &n) :
			storage_capacity(n)
	{
	}

	int storage_capacity;

	void save(fstream &lbfile)
	{
		save_common_component_properties<power_battery_component>(lbfile,
				*this);
		save_primitive<int>(lbfile, storage_capacity);
	}

	void load(fstream &lbfile)
	{
		load_common_component_properties<power_battery_component>(lbfile,
				*this);
		load_primitive<int>(lbfile, storage_capacity);
	}
};

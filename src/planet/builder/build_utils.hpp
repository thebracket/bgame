#pragma once

inline void add_construction(region_t &region, const int x, const int y, const int z, const std::string type, bool solid=false) {
	const int idx = mapidx(x,y,z);
	region.tile_flags[idx].set(CONSTRUCTION);
	if (solid) {
		region.solid[idx]=true;
		region.tile_flags[idx].reset(CAN_STAND_HERE);
	} else {
		region.solid[idx]=false;
		region.tile_flags[idx].set(CAN_STAND_HERE);
	}
	//region.tiles[idx].contents = get_tile_contents_index(type);

	if (type == "storage_locker") {
		auto cabinet = create_entity()
			->assign(position_t{ x,y,z })
			->assign(construct_container_t{});
		spawn_item_in_container(cabinet->id, "personal_survival_shelter_kit");
		spawn_item_in_container(cabinet->id, "personal_survival_shelter_kit");
		spawn_item_in_container(cabinet->id, "personal_survival_shelter_kit");
		spawn_item_in_container(cabinet->id, "camp_fire_kit");
		spawn_item_in_container(cabinet->id, "fire_axe");
		spawn_item_in_container(cabinet->id, "pickaxe");
	} else if (type == "cordex") {
		auto cordex = create_entity()
			->assign(position_t{ x,y,z })
			->assign(construct_power_t{10,1,0})
			->assign(viewshed_t{16, true});
	} else if (type == "solar_panel") {
		auto panel = create_entity()
			->assign(position_t{ x,y,z })
			->assign(construct_power_t{0,0,10});
	} else if (type == "rtg") {
		auto panel = create_entity()
			->assign(position_t{ x,y,z })
			->assign(construct_power_t{0,10,0});
	} else if (type == "battery") {
		auto panel = create_entity()
			->assign(position_t{ x,y,z })
			->assign(construct_power_t{50,0,0});
	} else if (type == "cryo_bed") {
		auto panel = create_entity()
			->assign(position_t{ x,y,z })
			->assign(construct_provides_sleep_t{});
	}
}


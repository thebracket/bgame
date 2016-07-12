#pragma once

inline int get_ground_z(region_t &region, const int x, const int y) {
	int z = REGION_DEPTH-1;
	bool hit_ground = false;
	while (!hit_ground) {
		const int idx = mapidx(x, y, z);
		if (region.solid[idx]) {
			hit_ground = true;
			++z;
		} else {
			--z;
		}
	}
	return z;
}

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

inline void set_tree_trunk(region_t &region, const int x, const int y, const int z, const int tree_id) {
	/*if (x>0 && y>0 && z>0 && x<REGION_WIDTH-1 && y<REGION_HEIGHT-1 && z<REGION_DEPTH-1) {
		const int idx = mapidx(x,y,z);
		region.tiles[idx].base_type = 7;
		region.tiles[idx].contents = 0;
		region.tiles[idx].flags.set(tile_flags::TREE);
		region.solid[idx] = true;
		region.tiles[idx].tree_id = tree_id;
	}*/
}

inline void set_tree_foliage(region_t &region, const int x, const int y, const int z, const int tree_id) {
	/*if (x>0 && y>0 && z>0 && x<REGION_WIDTH-1 && y<REGION_HEIGHT-1 && z<REGION_DEPTH-1) {
		const int idx = mapidx(x,y,z);
		region.tiles[idx].base_type = 8;
		region.tiles[idx].contents = 0;
		region.tiles[idx].flags.set(tile_flags::TREE);
		region.solid[idx] = true;
		region.tiles[idx].tree_id = tree_id;
	}*/
}

inline void plant_tree(region_t &region, const int x, const int y, const int z, random_number_generator &rng) {
	// Trees get their own entity
	const int tree_height = 1 + rng.roll_dice(2,4);
	for (int i=0; i<tree_height; ++i) {
		set_tree_trunk(region, x, y, z+i, region.next_tree_id);
		if ( i > 0) {
			const int radius = (tree_height - i)/2 + 1;
			for (int X=x-radius; X<x+radius; ++X) {
				for (int Y=y-radius; Y<y+radius; ++Y) {
					const float distance = distance2d(x,y,X,Y);
					if (distance <= radius && (X!=x || Y!=y)) {
						set_tree_foliage(region, X, Y, z+i, region.next_tree_id);
					}
				}
			}
		}
	}
	++region.next_tree_id;
}

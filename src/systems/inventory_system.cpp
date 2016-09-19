#include "inventory_system.hpp"
#include "../messages/messages.hpp"
#include "../components/components.hpp"
#include "movement_system.hpp"

void inventory_system::update(const double duration_ms) {
	// Do nothing!
}

void inventory_system::configure() {
	system_name = "Inventory";

	// Receive inventory change messages - refresh the inventory list
	subscribe<inventory_changed_message>([this](inventory_changed_message &msg) {
		dirty = true;
	});

	// Receive drop messages
	subscribe<drop_item_message>([this](drop_item_message &msg) {
		emit(item_claimed_message{msg.id, false});
		delete_component<item_carried_t>(msg.id);
		entity(msg.id)->component<item_t>()->claimed = false;
		entity(msg.id)->assign(position_t{ msg.x, msg.y, msg.z });
		entity_octree.add_node(octree_location_t{msg.x,msg.y,msg.z,msg.id});
		dirty = true;
	});

	// Receive pick-up messages
	subscribe<pickup_item_message>([this](pickup_item_message &msg) {
		position_t * pos = entity(msg.id)->component<position_t>();
		if (pos != nullptr) {
			entity_octree.remove_node(octree_location_t{pos->x,pos->y, pos->z,msg.id});
			delete_component<position_t>(msg.id);
		}
		delete_component<item_stored_t>(msg.id);
		entity(msg.id)->assign(item_carried_t{ msg.loc, msg.collector });
		dirty = true;
		emit(renderables_changed_message{});
	});

	// Receive item destruction messages
	subscribe<destroy_item_message>([this](destroy_item_message &msg) {
		position_t * pos = entity(msg.id)->component<position_t>();
		if (pos != nullptr) {
			entity_octree.remove_node(octree_location_t{pos->x,pos->y, pos->z,msg.id});
			delete_component<position_t>(msg.id);
		}

		delete_entity(msg.id);
	});

	// Receive claim messages - update an item as claimed/unclaimed
	subscribe<item_claimed_message>([] (item_claimed_message &msg) {
		entity_t * e = entity(msg.id);
		if (e) {
			item_t * item = e->component<item_t>();
			item->claimed = msg.claimed;
		}
	});

	// Receive build requests - claim components and add to the designations list.
	subscribe<build_request_message>([this] (build_request_message &msg) {
		if (!msg.building) return;

		// Claim components, create the designations
		available_building_t building = msg.building.get();

		// Build designation
		building_designation_t designate;
		designate.x = msg.x;
		designate.y = msg.y;
		designate.z = msg.z;
		designate.name = building.name;
		designate.tag = building.tag;
		designate.components = building.components;
    	designate.width = building.width;
    	designate.height = building.height;
    	designate.glyphs = building.glyphs;

		for (const std::string &requested_component : building.components) {
			const std::size_t component_id = claim_item_by_tag(requested_component);
			std::cout << "Component [" << requested_component << "] #" << component_id << "\n";
			designate.component_ids.push_back(std::make_pair(component_id, false));
		}

		auto building_template = create_entity()
			->assign(position_t{msg.x, msg.y, msg.z})
			->assign(building_t{ designate.tag, designate.width, designate.height, designate.glyphs, false });
		designate.building_entity = building_template->id;

		designations->buildings.push_back(designate);

		for (int x = designate.x; x < designate.x + designate.width; ++x) {
			for (int y=designate.y; y < designate.y + designate.height; ++y) {
				const int idx = mapidx(x,y,camera_position->region_z);
				current_region->tile_flags[idx].set(CONSTRUCTION);
			}
		}
	});
}

int item_category_available(const int &category) {
	int result = 0;
	each<item_t>([&result, &category] (entity_t &e, item_t &i) {
		if (i.category.test(category) && i.claimed == false) ++result;
	});
	return result;
}

bool is_item_category_available(const int &category) {
	return (item_category_available(category)>0);
}

bool is_ammo_available(const std::string &ammo_type) {
	int result = 0;
	each<item_t>([&result, &ammo_type] (entity_t &e, item_t &i) {
		if (i.category.test(WEAPON_AMMO) && i.claimed == false && item_defs.find(i.item_tag)->second.ammo == ammo_type) ++result;
	});
	return (result > 0);
}

std::size_t claim_closest_item_by_category(const int &category, position_t &pos) {
	// We're taking advantage of map being sorted to find the closest here
	std::map<float, std::size_t> distance_sorted; 

	each<item_t>([&distance_sorted, &category, &pos] (entity_t &e, item_t &i) {
		if (i.category.test(category) && i.claimed==false) {
			position_t * p = get_item_location(e.id);
			if (p) {
				const float distance = distance3d_squared(pos.x, pos.y, pos.z, p->x, p->y, p->z);
				distance_sorted[distance] = e.id;
			}
		}
	});

	if (distance_sorted.empty()) return 0;

	std::size_t closest_matching_id = distance_sorted.begin()->second;
	emit(item_claimed_message{closest_matching_id, true});

	return closest_matching_id;
}

std::size_t claim_closest_ammo(const int &category, position_t &pos, const std::string &ammo_type) {
	// We're taking advantage of map being sorted to find the closest here
	std::map<float, std::size_t> distance_sorted; 

	each<item_t>([&distance_sorted, &category, &pos, &ammo_type] (entity_t &e, item_t &i) {
		if (i.category.test(category) && i.claimed==false && item_defs.find(i.item_tag)->second.ammo == ammo_type) {
			position_t * p = get_item_location(e.id);
			if (p) {
				const float distance = distance3d_squared(pos.x, pos.y, pos.z, p->x, p->y, p->z);
				distance_sorted[distance] = e.id;
			}
		}
	});

	if (distance_sorted.empty()) return 0;

	std::size_t closest_matching_id = distance_sorted.begin()->second;
	emit(item_claimed_message{closest_matching_id, true});

	return closest_matching_id;
}

std::vector<available_building_t> get_available_buildings() {
	std::vector<available_building_t> result;

	std::unordered_map<std::string, int> existing_buildings;
	each<building_t>([&existing_buildings] (entity_t &e, building_t &b) {
		auto finder = existing_buildings.find(b.tag);
		if (finder == existing_buildings.end()) {
			existing_buildings[b.tag] = 1;
		} else {
			++finder->second;
		}
	});

	for (auto it = building_defs.begin(); it != building_defs.end(); ++it) {
		bool possible = true;

		// Evaluate the required components and see if they are available
		boost::container::flat_map<std::string, std::pair<int,int>> requirements;
		for (const std::string &require : it->second.components) {
			auto finder = requirements.find(require);
			if (finder == requirements.end()) {
				const int available_item_count = available_items_by_tag(require);
				requirements[require] = std::make_pair( 1, available_item_count );
				if (available_item_count < 1) possible = false;
			} else {
				++finder->second.first;
				if (finder->second.first > finder->second.second) possible=false;
			}
		}

		if (possible) {
			available_building_t building{it->second.name, it->second.tag};
			auto finder = existing_buildings.find(it->second.tag);
			if (finder == existing_buildings.end()) {
				building.n_existing = 0;
			} else {
				building.n_existing = finder->second;
			}

			building.height = it->second.height;
			building.width = it->second.width;
			building.components = it->second.components;
			building.glyphs = it->second.glyphs;
			result.push_back(building);
		}
	}

	return result;
}

std::vector<std::pair<std::string, std::string>> get_available_reactions() {
	std::vector<std::pair<std::string, std::string>> result;

	for (auto it=reaction_defs.begin(); it != reaction_defs.end(); ++it) {
		const std::string tag = it->first;
		const std::string workshop = it->second.workshop;
		const std::string name = it->second.name;

		if (!it->second.automatic) {
			bool possible = false;
			// Does a workshop exist?
			each<building_t>([&possible, &workshop] (entity_t &e, building_t &b) {
				if (b.complete && workshop == b.tag) possible = true;
			});

			// Do the components exist, and are unclaimed?
			if (possible) {
				boost::container::flat_map<std::string, std::pair<int,int>> requirements;
				for (const std::pair<std::string,int> &require : it->second.inputs) {
					auto finder = requirements.find(require.first);
					if (finder == requirements.end()) {
						const int available_components = available_items_by_tag(require.first);
						requirements[require.first] = std::make_pair(require.second, available_components);
						if (require.second > available_components) possible = false;
					} else {
						finder->second.first += require.second;
						if (finder->second.first > finder->second.second) possible = false;
					}
				}

				if (possible) {
					result.push_back(std::make_pair(tag, name));
				}
			}
		}
	}

	return result;
}

position_t * get_item_location(std::size_t id) {
	entity_t * e = entity(id);
	if (e == nullptr) return nullptr;
	position_t * pos = e->component<position_t>();

	if (pos == nullptr) {
		item_stored_t * stored = e->component<item_stored_t>();
		if (stored) {
			entity_t * container = entity(stored->stored_in);
			pos = container->component<position_t>();
		} else {
			item_carried_t * carried = e->component<item_carried_t>();
			if (carried) {
				entity_t * holder = entity(carried->carried_by);
				pos = holder->component<position_t>();
			}
		}
	}
	return pos;
}

void delete_item(const std::size_t &id) {
	emit(destroy_item_message{id});
}

int available_items_by_tag(const std::string &tag) {
	int result = 0;
	each<item_t>([&result, &tag] (entity_t &e, item_t &i) {
		if (i.item_tag == tag && i.claimed == false) ++result;
	});
	return result;
}

std::size_t claim_item_by_tag(const std::string &tag) {
	std::size_t result = 0;
	each<item_t>([&result, &tag] (entity_t &e, item_t &i) {
		if (i.item_tag == tag && i.claimed == false) result = e.id;
	});
	if (result != 0) {
		emit(item_claimed_message{result, true});
	}
	return result;
}

void unclaim_by_id(const std::size_t &id) {
	emit(item_claimed_message{id, false});	
}

bool is_better_armor(const std::string &item_tag, std::unordered_map<item_location_t, float> &ac_by_loc) {
	auto finder = clothing_types.find(item_tag);
	const float item_ac = finder->second.armor_class;
	item_location_t loc = INVENTORY;
	if (finder->second.slot == "head") loc = HEAD;
	if (finder->second.slot == "torso") loc = TORSO;
	if (finder->second.slot == "legs") loc = LEGS;
	if (finder->second.slot == "shoes") loc = FEET;

	auto tester = ac_by_loc.find(loc);
	if (tester == ac_by_loc.end()) {
		return true;
	} else {
		if (item_ac > tester->second) return true;
	}

	return false;
}

boost::optional<std::size_t> find_armor_upgrade(entity_t &E) {
	boost::optional<std::size_t> result;

	std::unordered_map<item_location_t, float> ac_by_loc;
	each<item_t, item_carried_t>([&ac_by_loc, &result, &E] (entity_t &e, item_t &i, item_carried_t &c) {
		if (c.carried_by == E.id && i.type == CLOTHING) {
			auto finder = clothing_types.find(i.item_tag);
			if (finder != clothing_types.end()) {
				ac_by_loc[c.location] = finder->second.armor_class;
			}
		} 
	});

	// Loop over all items on the ground
	each<item_t, position_t>([&result, &ac_by_loc] (entity_t &e, item_t &i, position_t &pos) {
		if (!i.claimed && i.type == CLOTHING) {
			if (is_better_armor(i.item_tag, ac_by_loc)) result = e.id;
		}
	});

	// ditto for all items in containers
	if (!result) {
		each<item_t, item_stored_t>([&result, &ac_by_loc] (entity_t &e, item_t &i, item_stored_t &pos) {
			if (!i.claimed && i.type == CLOTHING) {
				if (is_better_armor(i.item_tag, ac_by_loc)) result = e.id;
			}
		});
	}

	if (result) {
		entity(result.get())->component<item_t>()->claimed = true;
	}
	return result;
}

#include "inventory_system.hpp"
#include "../messages/messages.hpp"
#include "movement_system.hpp"
#include "../components/item_stored.hpp"
#include "../components/settler_ai.hpp"
#include "../raws/clothing.hpp"
#include <boost/container/flat_map.hpp>
#include "../raws/buildings.hpp"
#include "../raws/reactions.hpp"

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
		if (!entity(msg.id)) return;
		emit(item_claimed_message{msg.id, false});
		delete_component<item_carried_t>(msg.id);
		entity(msg.id)->component<item_t>()->claimed = false;
		entity(msg.id)->assign(position_t{ msg.x, msg.y, msg.z });
		entity_octree.add_node(octree_location_t{msg.x,msg.y,msg.z,msg.id});
		dirty = true;
	});

	// Receive pick-up messages
	subscribe<pickup_item_message>([this](pickup_item_message &msg) {
		if (!entity(msg.id)) return;
		auto pos = entity(msg.id)->component<position_t>();
		if (pos) {
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
		if (!entity(msg.id)) return;
		auto pos = entity(msg.id)->component<position_t>();
		if (pos) {
			entity_octree.remove_node(octree_location_t{pos->x,pos->y, pos->z,msg.id});
			delete_component<position_t>(msg.id);
		}

		delete_entity(msg.id);
	});

	// Receive claim messages - update an item as claimed/unclaimed
	subscribe<item_claimed_message>([] (item_claimed_message &msg) {
		auto e = entity(msg.id);
		if (e) {
			auto item = e->component<item_t>();
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

		for (const auto &requested_component : building.components) {
			const std::size_t component_id = claim_item_by_reaction_input(requested_component);
			std::cout << "Component [" << requested_component.tag << "] #" << component_id << "\n";
			designate.component_ids.push_back(std::make_pair(component_id, false));
		}

		auto building_template = create_entity()
			->assign(position_t{msg.x, msg.y, msg.z})
			->assign(building_t{ designate.tag, designate.width, designate.height, designate.glyphs, false });
		designate.building_entity = building_template->id;

		designations->buildings.push_back(designate);

        int sx = designate.x;
        int sy = designate.y;
        if (designate.width == 3) --sx;
        if (designate.height == 3) --sy;

		for (int x = sx; x < sx + designate.width; ++x) {
			for (int y=sy; y < sy + designate.height; ++y) {
				const auto idx = mapidx(x,y,camera_position->region_z);
				current_region->tile_flags[idx].set(CONSTRUCTION);
				current_region->tile_vegetation_type[idx] = 0;
                current_region->calc_render(idx);
			}
		}
	});

	subscribe<cancel_build_request_message>([this] (cancel_build_request_message &msg) {
		if (msg.building_entity == 0) return;

		// Unclaim components
		for (auto &d : designations->buildings) {
			if (d.building_entity == msg.building_entity) {
				for (auto &c : d.component_ids) {
					unclaim_by_id(c.first);
					each<position_t, item_carried_t>([&c] (entity_t &carrier, position_t &pos, item_carried_t &carried) {
						if (carrier.id == c.first) {
							emit(drop_item_message{c.first, pos.x, pos.y, pos.z});
						}
					});
				}
			}
		}

		// Remove any settler references to the building
		each<settler_ai_t>([&msg] (entity_t &e, settler_ai_t &ai) {
			if (ai.job_type_major == JOB_CONST && ai.building_target && ai.building_target->building_entity == msg.building_entity) {
				for (auto &c : ai.building_target->component_ids) {
					unclaim_by_id(c.first);
					each<position_t, item_carried_t>([&c] (entity_t &carrier, position_t &pos, item_carried_t &carried) {
						if (carrier.id == c.first) {
							emit(drop_item_message{c.first, pos.x, pos.y, pos.z});
						}
					});
				}
				ai.job_type_major = JOB_IDLE;
				ai.job_status = "Idle";
			}
		});

		// Erase from vector
		designations->buildings.erase(std::remove_if(
			designations->buildings.begin(),
			designations->buildings.end(),
			[&msg] (building_designation_t a) { return a.building_entity == msg.building_entity; }
		), designations->buildings.end());

		// Delete entity
		delete_entity(msg.building_entity);
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

std::size_t claim_closest_item_by_category(const int &category, position_t &pos, const int range) {
	// We're taking advantage of map being sorted to find the closest here
	std::map<float, std::size_t> distance_sorted; 

	each<item_t>([&distance_sorted, &category, &pos, &range] (entity_t &e, item_t &i) {
		if (i.category.test(category) && i.claimed==false) {
			auto p = get_item_location(e.id);
			if (p) {
				const float distance = distance3d_squared(pos.x, pos.y, pos.z, p->x, p->y, p->z);
				if (range == -1 || distance < range) distance_sorted[distance] = e.id;
			}
		}
	});

	if (distance_sorted.empty()) return 0;

	std::size_t closest_matching_id = distance_sorted.begin()->second;
	emit(item_claimed_message{closest_matching_id, true});

	return closest_matching_id;
}

std::size_t claim_closest_ammo(const int &category, position_t &pos, const std::string &ammo_type, const int range) {
	// We're taking advantage of map being sorted to find the closest here
	std::map<float, std::size_t> distance_sorted; 

	each<item_t>([&distance_sorted, &category, &pos, &ammo_type, &range] (entity_t &e, item_t &i) {
		if (i.category.test(category) && i.claimed==false && item_defs.find(i.item_tag)->second.ammo == ammo_type) {
			auto p = get_item_location(e.id);
			if (p) {
				const float distance = distance3d_squared(pos.x, pos.y, pos.z, p->x, p->y, p->z);
				if (range == -1 || distance < range) distance_sorted[distance] = e.id;
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

	boost::container::flat_map<std::string, int> existing_buildings;
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
		for (const auto &require : it->second.components) {
			auto finder = requirements.find(require.tag);
			if (finder == requirements.end()) {
				const int available_item_count = available_items_by_reaction_input(require);
				requirements[require.tag] = std::make_pair( require.quantity, available_item_count );
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
				for (const reaction_input_t &require : it->second.inputs) {
					auto finder = requirements.find(require.tag);
					if (finder == requirements.end()) {
						const int available_components = available_items_by_reaction_input(require);
						requirements[require.tag] = std::make_pair(require.quantity, available_components);
						if (require.quantity > available_components) possible = false;
					} else {
						finder->second.first += require.quantity;
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

boost::optional<position_t&> get_item_location(std::size_t id) {
	boost::optional<position_t&> result;

	auto e = entity(id);
	if (!e) return result;

	auto pos = e->component<position_t>();
	if (!pos)
	{
		auto stored = e->component<item_stored_t>();
		if (stored) {
			auto container = entity(stored->stored_in);
			if (container) {
				result = *container->component<position_t>();
			}
		} else {
			auto carried = e->component<item_carried_t>();
			if (carried) {
				auto holder = entity(carried->carried_by);
				if (holder) {
					result = *holder->component<position_t>();
				}
			}
		}
	} else {
		result = pos.get();
	}
	return result;
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

int available_items_by_reaction_input(const reaction_input_t &input) {
	int result = 0;
	each<item_t>([&result, &input] (entity_t &e, item_t &i) {
		if (i.item_tag == input.tag && i.claimed == false) {
			bool ok = true;
			if (input.required_material) {
				if (i.material != input.required_material.get()) ok=false;
			}
			if (input.required_material_type) {
				if (get_material(i.material)->spawn_type != input.required_material_type.get()) ok = false;
			}
			if (ok) ++result;
		}
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

std::size_t claim_item_by_reaction_input(const reaction_input_t &input) {
	std::size_t result = 0;
	each<item_t>([&result, &input] (entity_t &e, item_t &i) {
		if (i.item_tag == input.tag && i.claimed == false) {
			bool ok = true;
			if (input.required_material) {
				if (i.material != input.required_material.get()) ok=false;
			}
			if (input.required_material_type) {
				if (get_material(i.material)->spawn_type != input.required_material_type.get()) ok = false;
			}
			if (ok) result = e.id;
		}
	});
	if (result != 0) {
		emit(item_claimed_message{result, true});
	}
	return result;
}

void unclaim_by_id(const std::size_t &id) {
	emit(item_claimed_message{id, false});	
}

bool is_better_armor(const std::string &item_tag, boost::container::flat_map<item_location_t, float> &ac_by_loc) {
	auto finder = get_clothing_by_tag(item_tag);
	if (!finder) return false;

	const float item_ac = finder->armor_class;
	item_location_t loc = INVENTORY;
	if (finder->slot == "head") loc = HEAD;
	if (finder->slot == "torso") loc = TORSO;
	if (finder->slot == "legs") loc = LEGS;
	if (finder->slot == "shoes") loc = FEET;

	auto tester = ac_by_loc.find(loc);
	if (tester == ac_by_loc.end()) {
		return true;
	} else {
		if (item_ac > tester->second) return true;
	}

	return false;
}

boost::optional<std::size_t> find_armor_upgrade(entity_t &E, const int range) {
	boost::optional<std::size_t> result;

	auto my_pos = E.component<position_t>();
	if (range != -1 && !my_pos) return result;

	boost::container::flat_map<item_location_t, float> ac_by_loc;
	each<item_t, item_carried_t>([&ac_by_loc, &result, &E] (entity_t &e, item_t &i, item_carried_t &c) {
		if (c.carried_by == E.id && i.type == CLOTHING) {
			auto finder = get_clothing_by_tag(i.item_tag);
			if (finder) {
				ac_by_loc[c.location] = finder->armor_class;
			}
		} 
	});

	// Loop over all items on the ground
	each<item_t, position_t>([&result, &ac_by_loc, &my_pos, &range] (entity_t &e, item_t &i, position_t &pos) {
		if (!i.claimed && i.type == CLOTHING) {
			if (is_better_armor(i.item_tag, ac_by_loc) && (range==-1 || distance3d(my_pos->x, my_pos->y, my_pos->z, pos.x, pos.y, pos.z) < range)) result = e.id;
		}
	});

	// ditto for all items in containers
	if (!result) {
		each<item_t, item_stored_t>([&result, &ac_by_loc, &my_pos, &range] (entity_t &e, item_t &i, item_stored_t &pos) {
			if (!i.claimed && i.type == CLOTHING) {
				auto POS = e.component<position_t>();				
				if (is_better_armor(i.item_tag, ac_by_loc) && (range == -1 || (POS && distance3d(my_pos->x, my_pos->y, my_pos->z, POS->x, POS->y, POS->z) < range))) result = e.id;
			}
		});
	}

	if (result) {
		entity(result.get())->component<item_t>()->claimed = true;
	}
	return result;
}

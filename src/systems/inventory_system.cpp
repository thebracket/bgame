#include "inventory_system.hpp"
#include "../messages/messages.hpp"
#include "../components/components.hpp"
#include <boost/container/flat_map.hpp>
#include <boost/optional.hpp>
#include <array>
#include <map>

boost::container::flat_map<std::size_t, inventory_item_t> all_items;
std::array<int, NUMBER_OF_ITEM_CATEGORIES> item_availability;
std::array<std::vector<std::size_t>, NUMBER_OF_ITEM_CATEGORIES> items_by_category;

void inv_insert_or_update(inventory_item_t item) {
	if (item.id == 0) std::cout << "Warning: item id 0 is probably not valid for insertion!\n";
	auto finder = all_items.find(item.id);
	if (finder == all_items.end()) {
		// Insert it
		all_items[item.id] = item;
		for (int i=0; i<NUMBER_OF_ITEM_CATEGORIES; ++i) {
			if (item.categories.test(i)) {
				++item_availability[i];
				items_by_category[i].push_back(item.id);
			}
		}
	} else {
		if (item.pos) {
			finder->second.pos = item.pos;
		} else {
			finder->second.pos.reset();
		}
	}
}

void inventory_system::update(const double duration_ms) {
	if (dirty) {
		// Build the inventory list

		// Query items on the ground
		each<item_t, position_t>([] (entity_t &entity, item_t &item, position_t &pos) {
			inv_insert_or_update(inventory_item_t{ entity.id, false, pos, item.category, item.item_name, item.item_tag });
		});

		// Query items in storage containers
		each<item_t, item_stored_t>([] (entity_t &e, item_t &item, item_stored_t &stored) {
			const std::size_t stored_in = stored.stored_in;
			entity_t * container = entity(stored_in);
			position_t * pos = container->component<position_t>();

			inv_insert_or_update( inventory_item_t{ e.id, false, *pos, item.category, item.item_name, item.item_tag } );
		});

		// Query carried items
		each<item_t, item_carried_t>([] (entity_t &e, item_t &item, item_carried_t &stored) {
			const std::size_t stored_in = stored.carried_by;
			entity_t * container = entity(stored_in);
			position_t * pos = container->component<position_t>();

			inv_insert_or_update( inventory_item_t{ e.id, false, *pos, item.category, item.item_name, item.item_tag } );
		});

		/*
		for (int i=0; i<NUMBER_OF_ITEM_CATEGORIES; ++i)
			std::cout << "Available category " << i << " : " << item_availability[i] << "\n";
		*/
	}
	dirty = false;
}

void inventory_system::configure() {
	std::fill(item_availability.begin(), item_availability.end(), 0);

	// Receive inventory change messages - refresh the inventory list
	subscribe<inventory_changed_message>([this](inventory_changed_message &msg) {
		dirty = true;
	});

	// Receive claim messages - update an item as claimed/unclaimed
	subscribe<item_claimed_message>([] (item_claimed_message &msg) {
		std::cout << "Received item claimed message: " << msg.id << "," << msg.claimed << "\n";
		auto finder = all_items.find(msg.id);
		if (finder != all_items.end()) {
			finder->second.claimed = msg.claimed;
			for (int i=0; i<NUMBER_OF_ITEM_CATEGORIES; ++i) {
				if (finder->second.categories.test(i)) {
					if (msg.claimed) {
						--item_availability[i];
						std::cout << "Claimed - available by category: " << item_availability[i] << "\n";
					} else {
						++item_availability[i];
						std::cout << "Released - available by category: " << item_availability[i] << "\n";
					}
				}
			}
		}		
	});

	// Receive build requests - claim components and add to the designations list.
	subscribe<build_request_message>([this] (build_request_message &msg) {
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
			// Find the component and issue a claim for it
			for (auto it=all_items.begin(); it != all_items.end(); ++it) {
				if (it->second.item_tag == requested_component) {
					emit(item_claimed_message{ it->second.id, true });
					designate.component_ids.push_back(std::make_pair(it->second.id,false));
					std::cout << "Claimed component #" << it->second.id << "\n";
					break;
				}
			}
		}

		auto building_template = create_entity()
			->assign(position_t{msg.x, msg.y, msg.z})
			->assign(building_t{ designate.tag, designate.width, designate.height, designate.glyphs, false });
		designate.building_entity = building_template->id;

		designations->buildings.push_back(designate);

		for (int x = designate.x; x < designate.x + designate.width; ++x) {
			for (int y=designate.y; y < designate.y + designate.height; ++y) {
				const int idx = mapidx(x,y,camera_position->region_z);
				current_region->tiles[idx].flags.set(tile_flags::CONSTRUCTION);
			}
		}
	});
}

bool is_item_category_available(const int &category) {
	if (item_availability[category] > 0) return true;
	return false;
}

inventory_item_t claim_closest_item_by_category(const int &category, position_t &pos) {
	// We're taking advantage of map being sorted to find the closest here
	std::map<float, std::size_t> distance_sorted; 

	for (const int &id : items_by_category[category]) {
		auto f = all_items.find(id);
		if (f != all_items.end()) {
			if (f->second.claimed == false) {
				if (f->second.pos) {
					position_t pos2 = f->second.pos.get();
					float distance = rltk::distance3d( pos.x, pos.y, pos.z, pos2.x, pos2.y, pos2.z );
					distance_sorted[distance] = f->second.id;
				}
			}
		}
	}

	std::size_t closest_matching_id = distance_sorted.begin()->second;
	all_items[closest_matching_id].claimed = true;

	auto finder = all_items.find(closest_matching_id);
	if (finder != all_items.end()) {
		finder->second.claimed = true;
		for (int i=0; i<NUMBER_OF_ITEM_CATEGORIES; ++i) {
			if (finder->second.categories.test(i)) {
				--item_availability[i];
				std::cout << "Claimed - available by category: " << item_availability[i] << "\n";
			}
		}
	}
	std::cout << "Handing out tool with ID " << all_items[closest_matching_id].id << "\n";
	return all_items[closest_matching_id];
}

std::vector<available_building_t> get_available_buildings() {
	std::vector<available_building_t> result;

	for (auto it = building_defs.begin(); it != building_defs.end(); ++it) {
		bool possible = true;

		// Evaluate the required components and see if they are available
		boost::container::flat_map<std::string, int> requirements;
		for (const std::string &require : it->second.components) {
			auto finder = requirements.find(require);
			if (finder == requirements.end()) {
				requirements[require] = 1;
			} else {
				++finder->second;
			}
		}

		for (auto all = all_items.begin(); all != all_items.end(); ++all) {
			auto finder = requirements.find(all->second.item_tag);
			if (finder != requirements.end()) {
				--finder->second;
			}
		}

		for (auto req = requirements.begin(); req != requirements.end(); ++req) {
			if (req->second > 0) possible = false;
		}

		if (possible) {
			available_building_t building{it->second.name, it->second.tag};
			building.height = it->second.height;
			building.width = it->second.width;
			building.components = it->second.components;
			building.glyphs = it->second.glyphs;
			result.push_back(building);
		}
	}

	return result;
}

position_t get_item_location(std::size_t id) {
	auto finder = all_items.find(id);
	if (finder == all_items.end()) throw std::runtime_error("Unable to find item");
	return finder->second.pos.get();
}

void delete_item(const std::size_t &id) {
	auto finder = all_items.find(id);
	if (finder != all_items.end()) {
		if (finder->second.claimed) {
			for (int i=0; i<NUMBER_OF_ITEM_CATEGORIES; ++i) {
				if (finder->second.categories.test(i)) {
					++item_availability[i];
					items_by_category[i].erase(std::remove_if(items_by_category[i].begin(), items_by_category[i].end(), [&id] (std::size_t item) { return item == id; }), items_by_category[i].end());
				}
			}
		}
	}
	all_items.erase(id);
	delete_entity(id);
}

int available_items_by_tag(const std::string &tag) {
	int result = 0;
	for (auto item=all_items.begin(); item!=all_items.end(); ++item) {
		if (item->second.item_tag == tag && item->second.claimed != true) ++result;
	}
	return result;
}

std::size_t claim_item_by_tag(const std::string &tag) {
	for (auto item=all_items.begin(); item!=all_items.end(); ++item) {
		if (item->second.item_tag == tag && item->second.claimed == false) {
			item->second.claimed = true;
			for (int i=0; i<NUMBER_OF_ITEM_CATEGORIES; ++i) {
				if (item->second.categories.test(i)) {
					--item_availability[i];
				}
			}
			return item->second.id;
		}
	}
	return 0;
}

void unclaim_by_id(const std::size_t &id) {
	auto finder = all_items.find(id);
	if (finder != all_items.end()) {
		finder->second.claimed = false;
		for (int i=0; i<NUMBER_OF_ITEM_CATEGORIES; ++i) {
			++item_availability[i];
			std::cout << "Released - available by category: " << item_availability[i] << "\n";
		}
	}		
}

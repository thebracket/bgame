#include "inventory_assistant.hpp"
#include "../../bengine/ecs.hpp"
#include "../../components/item.hpp"
#include "../../components/building.hpp"
#include "../../components/item_stored.hpp"
#include "../../components/item_carried.hpp"
#include "../../components/position.hpp"
#include "../../components/claimed_t.hpp"
#include "../../raws/items.hpp"
#include "../../raws/defs/item_def_t.hpp"
#include "../../bengine/geometry.hpp"
#include "../../raws/buildings.hpp"
#include "../../raws/reactions.hpp"
#include "../../raws/materials.hpp"
#include "../../raws/defs/building_def_t.hpp"
#include "../../raws/reactions.hpp"
#include "../../raws/defs/material_def_t.hpp"
#include "../../raws/clothing.hpp"
#include "../../raws/defs/clothing_t.hpp"
#include "../../raws/defs/reaction_t.hpp"
#include "../ai/inventory_system.hpp"

using namespace bengine;
using namespace buildings;

namespace inventory {

	int item_category_available(const int &category) {
		int result = 0;
		each<item_t>([&result, &category](bengine::entity_t &e, item_t &i) {
			if (i.category.test(category) && e.component<claimed_t>() == nullptr) ++result;
		});
		return result;
	}

	int blocks_available() {
		int result = 0;
		each<item_t>([&result](bengine::entity_t &e, item_t &i) {
			if (i.item_tag == "block" && e.component<claimed_t>() == nullptr) ++result;
		});
		return result;
	}

	bool is_item_category_available(const int &category) {
		return (item_category_available(category)>0);
	}

	bool is_ammo_available(const std::string &ammo_type) {
		int result = 0;
		each<item_t>([&result, &ammo_type](bengine::entity_t &e, item_t &i) {
			if (i.category.test(WEAPON_AMMO) && e.component<claimed_t>() == nullptr && get_item_def(i.item_tag)->ammo == ammo_type) ++result;
		});
		return (result > 0);
	}

	std::size_t claim_closest_item_by_category(const int &category, position_t &pos, const int range) {
		// We're taking advantage of map being sorted to find the closest here
		std::map<float, std::size_t> distance_sorted;

		each<item_t>([&distance_sorted, &category, &pos, &range](bengine::entity_t &e, item_t &i) {
			if (i.category.test(category) && e.component<claimed_t>() == nullptr) {
				auto p = get_item_location(e.id);
				if (p) {
					const float distance = distance3d_squared(pos.x, pos.y, pos.z, p->x, p->y, p->z);
					if (range == -1 || distance < range) distance_sorted[distance] = e.id;
				}
			}
		});

		if (distance_sorted.empty()) return 0;

		std::size_t closest_matching_id = distance_sorted.begin()->second;
		systems::inventory_system::claim_item(closest_matching_id, true );

		return closest_matching_id;
	}

	std::size_t claim_closest_ammo(const int &category, position_t &pos, const std::string &ammo_type, const int range) {
		// We're taking advantage of map being sorted to find the closest here
		std::map<float, std::size_t> distance_sorted;

		each<item_t>([&distance_sorted, &category, &pos, &ammo_type, &range](bengine::entity_t &e, item_t &i) {
			if (i.category.test(category) && e.component<claimed_t>() == nullptr && get_item_def(i.item_tag)->ammo == ammo_type) {
				auto p = get_item_location(e.id);
				if (p) {
					const float distance = distance3d_squared(pos.x, pos.y, pos.z, p->x, p->y, p->z);
					if (range == -1 || distance < range) distance_sorted[distance] = e.id;
				}
			}
		});

		if (distance_sorted.empty()) return 0;

		std::size_t closest_matching_id = distance_sorted.begin()->second;
		systems::inventory_system::claim_item(closest_matching_id, true );

		return closest_matching_id;
	}

	std::vector<available_building_t> get_available_buildings() {
		std::vector<available_building_t> result;

		std::unordered_map<std::string, int> existing_buildings;
		each<building_t>([&existing_buildings](bengine::entity_t &e, building_t &b) {
			auto finder = existing_buildings.find(b.tag);
			if (finder == existing_buildings.end()) {
				existing_buildings[b.tag] = 1;
			}
			else {
				++finder->second;
			}
		});

		each_building_def([&result, &existing_buildings](building_def_t * it) {
			bool possible = true;

			// Evaluate the required components and see if they are available
			std::unordered_map<std::string, std::pair<int, int>> requirements;
			for (const auto &require : it->components) {
				auto finder = requirements.find(require.tag);
				if (finder == requirements.end()) {
					const int available_item_count = available_items_by_reaction_input(require);
					requirements[require.tag] = std::make_pair(require.quantity, available_item_count);
					if (available_item_count < 1) possible = false;
				}
				else {
					++finder->second.first;
					if (finder->second.first > finder->second.second) possible = false;
				}
			}

			if (possible) {
				available_building_t building{ it->name, it->tag };
				if (it->structure) building.structure = true;
				auto finder = existing_buildings.find(it->tag);
				if (finder == existing_buildings.end()) {
					building.n_existing = 0;
				}
				else {
					building.n_existing = finder->second;
				}

				building.height = it->height;
				building.width = it->width;
				building.components = it->components;
				//building.glyphs = it->glyphs;
				//building.glyphs_ascii = it->glyphs_ascii;
				result.emplace_back(building);
			}
		});

		std::sort(result.begin(), result.end(), [](auto &b1, auto &b2) { return b1.get_name() < b2.get_name(); });
		return result;
	}

	std::vector<std::pair<std::string, std::string>> get_available_reactions() {
		std::vector<std::pair<std::string, std::string>> result;

		each_reaction([&result](std::string rtag, reaction_t * it) {
			const std::string tag = rtag;
			const std::string workshop = it->workshop;
			const std::string name = it->name;

			if (!it->automatic) {
				bool possible = false;
				// Does a workshop exist?
				each<building_t>([&possible, &workshop](bengine::entity_t &e, building_t &b) {
					if (b.complete && workshop == b.tag) possible = true;
				});

				// Do the components exist, and are unclaimed?
				if (possible) {
					std::unordered_map<std::string, std::pair<int, int>> requirements;
					for (const reaction_input_t &require : it->inputs) {
						auto finder = requirements.find(require.tag);
						if (finder == requirements.end()) {
							const int available_components = available_items_by_reaction_input(require);
							requirements[require.tag] = std::make_pair(require.quantity, available_components);
							if (require.quantity > available_components) possible = false;
						}
						else {
							finder->second.first += require.quantity;
							if (finder->second.first > finder->second.second) possible = false;
						}
					}

					if (possible) {
						result.push_back(std::make_pair(tag, name));
					}
				}
			}
		});

		std::sort(result.begin(), result.end(), [](auto &b1, auto &b2) { return b1.second < b2.second; });
		return result;
	}

	position_t * get_item_location(std::size_t id) {
		auto e = bengine::entity(id);
		if (!e) return nullptr;

		auto pos = e->component<position_t>();
		if (!pos)
		{
			auto stored = e->component<item_stored_t>();
			if (stored) {
				auto container = bengine::entity(stored->stored_in);
				if (container) {
					return container->component<position_t>();
				}
			}
			else {
				auto carried = e->component<item_carried_t>();
				if (carried) {
					auto holder = bengine::entity(carried->carried_by);
					if (holder) {
						return holder->component<position_t>();
					}
				}
			}
		}
		else {
			return pos;
		}
		return nullptr;
	}

	void delete_item(const std::size_t &id) {
		systems::inventory_system::destroy_item(id);
	}

	int available_items_by_tag(const std::string &tag) {
		int result = 0;
		each<item_t>([&result, &tag](bengine::entity_t &e, item_t &i) {
			if (i.item_tag == tag && e.component<claimed_t>() == nullptr) ++result;
		});
		return result;
	}

	int available_items_by_reaction_input(const reaction_input_t &input) {
		int result = 0;
		//std::cout << "Looking for item type: " << input.tag << "\n";
		each<item_t>([&result, &input](bengine::entity_t &e, item_t &i) {
			//std::cout << "Evaluating " << i.item_tag << "\n";
			if ((input.tag == "any" || i.item_tag == input.tag)) {
				//std::cout << "Available items tag hit - " << input.tag << "\n";
				if (e.component<claimed_t>() == nullptr) {
					bool ok = true;
					if (input.required_material != 0) {
						if (i.material != input.required_material) {
							//std::cout << "Reject item by material type\n";
							ok = false;
						}
					}
					if (input.required_material_type != no_spawn_type) {
						if (get_material(i.material)->spawn_type != input.required_material_type) {
							//std::cout << "Reject item by spawn type\n";
							ok = false;
						}
					}
					if (ok) ++result;
				}
				else {
					//std::cout << "Reject item because it was claimed\n";
				}
			}
		});
		//std::cout << "Returning available item count: " << result << "\n";
		return result;
	}

	std::size_t claim_item_by_tag(const std::string &tag) {
		std::size_t result = 0;
		each<item_t>([&result, &tag](bengine::entity_t &e, item_t &i) {
			if (i.item_tag == tag && e.component<claimed_t>() == nullptr) result = e.id;
		});
		if (result != 0) {
			systems::inventory_system::claim_item(result, true );
		}
		return result;
	}

	std::size_t claim_item_by_reaction_input(const reaction_input_t &input, bool really_claim) {
		std::size_t result = 0;
		each<item_t>([&result, &input](bengine::entity_t &e, item_t &i) {
			if ((input.tag == "any" || i.item_tag == input.tag) && e.component<claimed_t>() == nullptr) {
				bool ok = true;
				if (input.required_material != 0) {
					if (i.material != input.required_material) ok = false;
				}
				if (input.required_material_type != no_spawn_type) {
					if (get_material(i.material)->spawn_type != input.required_material_type) ok = false;
				}
				if (ok) result = e.id;
			}
		});
		if (result != 0 && really_claim) {
			systems::inventory_system::claim_item(result, true);
		}
		return result;
	}

	void unclaim_by_id(const std::size_t &id) {
		systems::inventory_system::claim_item(id, false);
	}

	bool is_better_armor(const std::string &item_tag, std::unordered_map<int, float> &ac_by_loc) {
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
		}
		else {
			if (item_ac > tester->second) return true;
		}

		return false;
	}

	std::size_t find_armor_upgrade(bengine::entity_t &E, const int range) {
		auto my_pos = E.component<position_t>();
		if (range != -1 && !my_pos) return 0;
		std::size_t result = 0;

		std::unordered_map<int, float> ac_by_loc;
		each<item_t, item_carried_t>([&ac_by_loc, &result, &E](bengine::entity_t &e, item_t &i, item_carried_t &c) {
			if (c.carried_by == E.id && i.type == CLOTHING) {
				auto finder = get_clothing_by_tag(i.item_tag);
				if (finder) {
					ac_by_loc[c.location] = finder->armor_class;
				}
			}
		});

		// Loop over all items on the ground
		each<item_t, position_t>([&result, &ac_by_loc, &my_pos, &range](bengine::entity_t &e, item_t &i, position_t &pos) {
			if (e.component<claimed_t>() == nullptr && i.type == CLOTHING) {
				if (is_better_armor(i.item_tag, ac_by_loc) && (range == -1 || distance3d(my_pos->x, my_pos->y, my_pos->z, pos.x, pos.y, pos.z) < range)) result = e.id;
			}
		});

		// ditto for all items in containers
		if (!result) {
			each<item_t, item_stored_t>([&result, &ac_by_loc, &my_pos, &range](bengine::entity_t &e, item_t &i, item_stored_t &pos) {
				if (e.component<claimed_t>() == nullptr && i.type == CLOTHING) {
					auto POS = e.component<position_t>();
					if (is_better_armor(i.item_tag, ac_by_loc) && (range == -1 || (POS && distance3d(my_pos->x, my_pos->y, my_pos->z, POS->x, POS->y, POS->z) < range))) result = e.id;
				}
			});
		}

		if (result != 0) {
			bengine::entity(result)->assign(claimed_t{E.id});
		}
		return result;
	}
} // End namespace
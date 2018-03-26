#include "stdafx.h"
#include "../helpers/workflow_assistant.hpp"
#include "../../raws/buildings.hpp"
#include "../../raws/reactions.hpp"
#include "../../raws/defs/reaction_t.hpp"
#include "../../global_assets/building_designations.hpp"

namespace systems {
	namespace workflow_system {

		using namespace bengine;

		bool dirty = true;

		void update_workflow() {
			dirty = true;
		}

		void run(const double &duration_ms) {
			using namespace workflow;

			if (dirty) {
				automatic_reactions.clear();

				// Enumerate buildings and see which ones have reactions.
				each<position_t, building_t>([](entity_t &e, position_t &pos, building_t &b) {
					if (b.complete) {
						auto finder = get_reactions_for_building(b.tag);
						if (!finder.empty()) {
							for (const std::string &reaction_name : finder) {
								auto reactor = get_reaction_def(reaction_name);
								if (reactor != nullptr) {

									// Automatic reactions are added to the auto reactor list
									if (reactor->automatic) {
										auto automatic_finder = automatic_reactions.find(e.id);
										if (automatic_finder == automatic_reactions.end()) {
											automatic_reactions[e.id] = std::vector<std::string>{ reaction_name };
										}
										else {
											automatic_finder->second.push_back(reaction_name);
										}
									}
								}
							}
						}
					}
				});				

				// Not dirty anymore!
				dirty = false;
			}

			// Erase all completed jobs
			building_designations->build_orders.erase(
				std::remove_if(building_designations->build_orders.begin(),
					building_designations->build_orders.end(),
					[](auto order_pair) { return order_pair.first < 1; }),
				building_designations->build_orders.end());
		}
	}
}

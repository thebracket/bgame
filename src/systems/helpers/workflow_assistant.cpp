#include "workflow_assistant.hpp"
#include "../../raws/reactions.hpp"
#include "../../raws/defs/reaction_t.hpp"
#include "inventory_assistant.hpp"
#include "../../global_assets/game_designations.hpp"
#include "../../components/building.hpp"
#include "../ai/workflow_system.hpp"

namespace workflow {
	std::unordered_map<std::size_t, std::vector<std::string>> automatic_reactions;
	std::unordered_set<std::size_t> workshop_claimed;

	using namespace bengine;

	bool is_auto_reaction_task_available(const settler_ai_t &ai) {
		if (automatic_reactions.empty()) return false;

		for (auto outerit = automatic_reactions.begin(); outerit != automatic_reactions.end(); ++outerit) {
			// Is the workshop busy?
			auto busy_finder = workshop_claimed.find(outerit->first);
			if (busy_finder == workshop_claimed.end()) {
				// Iterate available automatic reactions
				for (const std::string &reaction_name : outerit->second) {
					auto reaction = get_reaction_def(reaction_name);
					if (reaction != nullptr) {
						// Are the inputs available?
						bool available = true;
						for (auto &input : reaction->inputs) {
							const int n_available = inventory::available_items_by_reaction_input(input);
							if (n_available < input.quantity) {
								available = false;
							};
						}

						if (available) {
							// Components are available, build job and return it
							return true;
						}
					}
				}
			}
		}

		return false;
	}

	std::unique_ptr<reaction_task_t> find_automatic_reaction_task(const ai_tag_work_order &ai) {
		if (automatic_reactions.empty()) return std::unique_ptr<reaction_task_t>{};

		std::unique_ptr<reaction_task_t> result;

		// Iterate through available reactions
		for (auto outerit = automatic_reactions.begin(); outerit != automatic_reactions.end(); ++outerit) {
			// Is the workshop busy?
			auto busy_finder = workshop_claimed.find(outerit->first);
			if (busy_finder == workshop_claimed.end()) {
				// Iterate available automatic reactions
				for (const std::string &reaction_name : outerit->second) {
					auto reaction = get_reaction_def(reaction_name);
					if (reaction != nullptr) {
						// Are the inputs available?
						bool available = true;
						std::vector<std::pair<std::size_t, bool>> components;
						for (auto &input : reaction->inputs) {
							const int n_available = inventory::available_items_by_reaction_input(input);
							if (n_available < input.quantity) {
								available = false;
							}
							else {
								// Claim an item and push its # to the list
								std::size_t item_id = inventory::claim_item_by_reaction_input(input);
								components.push_back(std::make_pair(item_id, false));
							}
						}

						if (available) {
							// Components are available, build job and return it
							result = std::make_unique<reaction_task_t>(outerit->first, reaction->name, reaction->tag, components);
							workshop_claimed.insert(outerit->first);
							return result;
						}
						else {
							for (auto comp : components) {
								inventory::unclaim_by_id(comp.first);
							}
						}
					}
				}
			}
		}

		return result;
	}

	std::unique_ptr<reaction_task_t> find_queued_reaction_task(const ai_tag_work_order &ai) {
		if (designations->build_orders.empty()) return std::unique_ptr<reaction_task_t>();

		std::unique_ptr<reaction_task_t> result;

		// Iterate through queued jobs
		for (std::pair<uint8_t, std::string> &order : designations->build_orders) {
			auto reaction = get_reaction_def(order.second);

			// Is there an available workshop of the right type?
			bool possible = false;
			std::size_t workshop_id;
			each<building_t>([&possible, &reaction, &workshop_id](entity_t &e, building_t &b) {
				if (b.complete && b.tag == reaction->workshop) {
					auto busy_finder = workshop_claimed.find(e.id);
					if (busy_finder == workshop_claimed.end()) {
						workshop_id = e.id;
						possible = true;
					}
				}
			});
			if (!possible) break;

			// Is the settler allowed to do this?
			int target_category = -1;
			if (reaction->skill == "Carpentry") {
				target_category = JOB_CARPENTRY;
			}
			else if (reaction->skill == "Masonry") {
				target_category = JOB_MASONRY;
			}
			if (target_category > -2) {
				bool available = true;
				std::vector<std::pair<std::size_t, bool>> components;
				for (auto &input : reaction->inputs) {
					const int n_available = inventory::available_items_by_reaction_input(input);
					if (n_available < input.quantity) {
						available = false;
					}
					else {
						// Claim an item and push its # to the list
						std::size_t item_id = inventory::claim_item_by_reaction_input(input);
						components.push_back(std::make_pair(item_id, false));
					}
				}

				if (available) {
					// Components are available, build job and return it
					result = std::make_unique<reaction_task_t>(workshop_id, reaction->name, reaction->tag, components);
					workshop_claimed.insert(workshop_id);
					--order.first;
					systems::workflow_system::update_workflow();
					return result;
				}
				else {
					for (auto comp : components) {
						inventory::unclaim_by_id(comp.first);
					}
				}
			}
		}

		return result;
	}

	void free_workshop(const std::size_t &id) {
		workshop_claimed.erase(id);
	}

}
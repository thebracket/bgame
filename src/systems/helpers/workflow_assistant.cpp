#include "stdafx.h"
#include "workflow_assistant.hpp"
#include "../../raws/reactions.hpp"
#include "../../raws/defs/reaction_t.hpp"
#include "inventory_assistant.hpp"
#include "../ai/workflow_system.hpp"
#include "../../global_assets/building_designations.hpp"

namespace workflow {
	std::unordered_map<std::size_t, std::vector<std::string>> automatic_reactions;

	using namespace bengine;

	bool is_auto_reaction_task_available(const int worker_id, const settler_ai_t &ai) noexcept {
		if (automatic_reactions.empty()) return false;

		for (const auto &outerit : automatic_reactions) {
			// Is the workshop busy?
			const auto workshop_entity = entity(outerit.first);
			if (workshop_entity)
			{
				// Iterate available automatic reactions
				for (const auto &reaction_name : outerit.second) {
					auto reaction = get_reaction_def(reaction_name);
					if (reaction != nullptr) {
						// Are the inputs available?
						auto available = true;
						for (auto &input : reaction->inputs) {
							const auto n_available = inventory::available_items_by_reaction_input(worker_id, input);
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

	std::unique_ptr<reaction_task_t> find_automatic_reaction_task(const int worker_id, const ai_tag_work_order &ai) noexcept {
		if (automatic_reactions.empty()) return std::unique_ptr<reaction_task_t>{};

		std::unique_ptr<reaction_task_t> result;

		// Iterate through available reactions
		for (const auto &outerit : automatic_reactions) {
			// Is the workshop busy?
			const auto workshop_entity = entity(outerit.first);
			if (workshop_entity)
			{
				// Iterate available automatic reactions
				for (const auto &reaction_name : outerit.second) {
					const auto reaction = get_reaction_def(reaction_name);
					if (reaction != nullptr) {
						// Are the inputs available?
						auto available = true;
						std::vector<std::pair<int, bool>> components;
						for (auto &input : reaction->inputs) {
							const auto n_available = inventory::available_items_by_reaction_input(worker_id, input);
							if (n_available < input.quantity) {
								available = false;
							}
							else {
								// Claim an item and push its # to the list
								const auto item_id = inventory::claim_item_by_reaction_input(input, worker_id);
								components.emplace_back(std::make_pair(item_id, false));
							}
						}

						if (available) {
							// Components are available, build job and return it
							result = std::make_unique<reaction_task_t>(outerit.first, reaction->name, reaction->tag, components);
							return result;
						}
						else {
							for (const auto comp : components) {
								inventory::unclaim_by_id(comp.first);
							}
						}
					}
				}
			}
		}

		return result;
	}

	std::unique_ptr<reaction_task_t> find_queued_reaction_task(const int worker_id, const ai_tag_work_order &ai) noexcept {
		if (building_designations->build_orders.empty()) return std::unique_ptr<reaction_task_t>();

		std::unique_ptr<reaction_task_t> result;

		// Iterate through queued jobs
		for (auto &order : building_designations->build_orders) {
			auto reaction = get_reaction_def(order.second);

			// Is there an available workshop of the right type?
			auto possible = false;
			std::size_t workshop_id;
			each<building_t>([&possible, &reaction, &workshop_id](entity_t &e, building_t &b) {
				if (b.complete && b.tag == reaction->workshop) {
					workshop_id = e.id;
					possible = true;
				}
			});
			if (!possible) break;

			// Is the settler allowed to do this?
			auto target_category = -1;
			if (reaction->skill == "Carpentry") {
				target_category = JOB_CARPENTRY;
			}
			else if (reaction->skill == "Masonry") {
				target_category = JOB_MASONRY;
			}
			if (target_category > -2) {
				auto available = true;
				std::vector<std::pair<int, bool>> components;
				for (auto &input : reaction->inputs) {
					const auto n_available = inventory::available_items_by_reaction_input(worker_id, input);
					if (n_available < input.quantity) {
						available = false;
					}
					else {
						// Claim an item and push its # to the list
						auto item_id = inventory::claim_item_by_reaction_input(input, worker_id);
						components.emplace_back(std::make_pair(item_id, false));
					}
				}

				if (available) {
					// Components are available, build job and return it
					result = std::make_unique<reaction_task_t>(workshop_id, reaction->name, reaction->tag, components);
					--order.first;
					systems::workflow_system::update_workflow();
					return result;
				}
				else {
					for (const auto &comp : components) {
						inventory::unclaim_by_id(comp.first);
					}
				}
			}
		}

		return result;
	}

	void free_workshop(const std::size_t &id) noexcept {
		delete_component<claimed_t>(id);
	}

}

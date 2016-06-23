#include "workflow_system.hpp"
#include "../messages/messages.hpp"
#include "../raws/raws.hpp"
#include "../components/components.hpp"
#include <boost/container/flat_map.hpp>
#include <boost/container/flat_set.hpp>
#include "inventory_system.hpp"

boost::container::flat_map<std::size_t, std::vector<std::string>> automatic_reactions;
boost::container::flat_set<std::size_t> workshop_claimed;

void workflow_system::update(const double duration_ms) {
    if (dirty) {
        automatic_reactions.clear();

        // Enumerate buildings and see which ones have reactions.
        each<position_t, building_t>([this] (entity_t &e, position_t &pos, building_t &b) {
            if (b.complete) {
                auto finder = reaction_building_defs.find(b.tag);
                if (finder != reaction_building_defs.end()) {
                    for (const std::string &reaction_name : finder->second) {
                        auto reactor = reaction_defs.find(reaction_name);
                        if (reactor != reaction_defs.end()) {

                            // Automatic reactions are added to the auto reactor list
                            if (reactor->second.automatic) {
                                auto automatic_finder = automatic_reactions.find(e.id);
                                if (automatic_finder == automatic_reactions.end()) {
                                    automatic_reactions[e.id] = std::vector<std::string>{ reaction_name };
                                } else {
                                    automatic_finder->second.push_back(reaction_name);
                                }
                            } else {
                                // TODO: Handle other reactions
                            }
                        }
                    }
                }
            }
        });
        dirty = false;
    }
}

void workflow_system::configure() {
    subscribe<update_workflow_message>([this] (update_workflow_message &msg) {
		dirty = true;
	});
}

boost::optional<reaction_task_t> find_automatic_reaction_task(const settler_ai_t &ai) {
    if (automatic_reactions.empty()) return boost::optional<reaction_task_t>{};

    boost::optional<reaction_task_t> result;

    // Iterate through available reactions
    for (auto outerit=automatic_reactions.begin(); outerit != automatic_reactions.end(); ++outerit) {
        // Is the workshop busy?
        auto busy_finder = workshop_claimed.find(outerit->first);
        if (busy_finder == workshop_claimed.end()) {
            // Iterate available automatic reactions
            for (const std::string &reaction_name : outerit->second) {
                auto reaction = reaction_defs.find(reaction_name);
                if (reaction != reaction_defs.end()) {
                    // Is the settler allowed to do this?
                    int target_category = -1;
                    if (reaction->second.skill == "Carpentry") {
                        target_category = JOB_CARPENTRY;
                    } else if (reaction->second.skill == "Masonry") {
                        target_category = JOB_MASONRY;
                    }
                    if (target_category == -1 || ai.permitted_work[target_category]) {
                        // Are the inputs available?
                        bool available = true;
                        std::vector<std::pair<std::size_t,bool>> components;
                        for (auto &input : reaction->second.inputs) {
                            const int n_available = available_items_by_tag(input.first);
                            if (n_available < input.second) {
                                available = false;
                            } else {
                                // Claim an item and push its # to the list
                                std::size_t item_id = claim_item_by_tag(input.first);
                                components.push_back(std::make_pair(item_id,false));
                            }
                        }

                        if (available) {
                            // Components are available, build job and return it
                            result = reaction_task_t{outerit->first, reaction->second.name, reaction->second.tag, components};
                            workshop_claimed.insert(outerit->first);
                            return result;
                        } else {
                            for (auto comp : components) {
                                unclaim_by_id(comp.first);
                            }
                        }
                    }
                }
            }
        }
    }

    return result;
}

void free_workshop(const std::size_t &id) {
    workshop_claimed.erase(id);
}

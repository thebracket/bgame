#include "distance_map_system.hpp"
#include "../../components/grazer_ai.hpp"
#include "../../components/corpse_harvestable.hpp"
#include "../../planet/region.hpp"
#include "../../messages/entity_moved_message.hpp"
#include "../../components/construct_provides_sleep.hpp"
#include "../../components/settler_ai.hpp"
#include "../../messages/messages.hpp"
#include "../../components/item.hpp"
#include "../../components/claimed_t.hpp"
#include "inventory_system.hpp"
#include "../../components/item_stored.hpp"
#include "../../main/game_designations.hpp"

dijkstra_map huntables_map;
dijkstra_map butcherables_map;
dijkstra_map bed_map;
dijkstra_map settler_map;
dijkstra_map architecure_map;
dijkstra_map blocks_map;
dijkstra_map levers_map;
dijkstra_map axe_map;
dijkstra_map pick_map;
dijkstra_map harvest_map;
bool dijkstra_debug = false;

using namespace rltk;

namespace dijkstra {
    void update_hunting_map() {
        std::vector<int> huntables;
        each<grazer_ai, position_t>([&huntables] (entity_t &e, grazer_ai &ai, position_t &pos) {
            huntables.emplace_back(mapidx(pos));
        });
        huntables_map.update(huntables);
    }

    void update_butcher_map() {
        std::vector<int> butcherables;
        each<corpse_harvestable, position_t>([&butcherables] (entity_t &e, corpse_harvestable &corpse, position_t &pos) {
            butcherables.emplace_back(mapidx(pos));
        });
        butcherables_map.update(butcherables);
    }

    void update_bed_map() {
        std::vector<int> beds;
        each<construct_provides_sleep_t, position_t>([&beds] (entity_t &e, construct_provides_sleep_t &bed, position_t &pos) {
            auto claimed = e.component<claimed_t>();
            if (claimed == nullptr) {
                beds.emplace_back(mapidx(pos));
            }
        });
        bed_map.update(beds);
    }

    void update_settler_map() {
        std::vector<int> settlers;
        each<settler_ai_t, position_t>([&settlers] (entity_t &e, settler_ai_t &settler, position_t &pos) {
            settlers.emplace_back(mapidx(pos));
        });
        settler_map.update(settlers);
    }

    void update_architecure_map() {
        std::vector<int> targets;
        for (auto it = designations->architecture.begin(); it != designations->architecture.end(); ++it) {
            targets.emplace_back(it->first);
        }
        architecure_map.update_architecture(targets);
    }

    void update_blocks_map() {
        std::unordered_set<int> used;
        std::vector<int> targets;

        each_if<item_t>([] (entity_t &e, item_t &i) { return i.claimed == false && i.item_tag == "block"; },
                        [&used, &targets] (entity_t &e, item_t &i) {
                            auto pos = get_item_location(e.id);
                            if (pos) {
                                const int idx = mapidx(*pos);
                                if (used.find(idx)==used.end()) {
                                    used.insert(idx);
                                    targets.emplace_back(idx);
                                }
                            }
                        }
        );
        blocks_map.update(targets);
    }

    void update_levers_map() {
        std::vector<int> targets;
        for (auto it = designations->levers_to_pull.begin(); it != designations->levers_to_pull.end(); ++it) {
            auto lever_id = *it;
            auto lever_entity = entity(lever_id);
            if (!lever_entity) break;
            auto lever_pos = lever_entity->component<position_t>();
            if (!lever_pos) break;
            targets.emplace_back(mapidx(*lever_pos));
        }
        levers_map.update(targets);
    }

    void update_axe_map() {
        std::vector<int> targets;
        each<item_t>([&targets] (entity_t &e, item_t &item) {
            if (!item.category.test(TOOL_CHOPPING)) return; // Not an axe!
            if (e.component<claimed_t>() != nullptr) return; // Don't touch claimed items
            if (item.claimed) return;

            auto pos = e.component<position_t>();
            if (pos != nullptr) {
                targets.emplace_back(mapidx(*pos));
            } else {
                auto store = e.component<item_stored_t>();
                if (store != nullptr) {
                    auto storage_entity = entity(store->stored_in);
                    if (storage_entity) {
                        auto spos = storage_entity->component<position_t>();
                        targets.emplace_back(mapidx(*spos));
                    }
                }
            }
        });
        axe_map.update(targets);
    }

    void update_pick_map() {
        std::vector<int> targets;
        each<item_t>([&targets] (entity_t &e, item_t &item) {
            if (!item.category.test(TOOL_DIGGING)) return; // Not an axe!
            if (e.component<claimed_t>() != nullptr) return; // Don't touch claimed items
            if (item.claimed) return;

            auto pos = e.component<position_t>();
            if (pos != nullptr) {
                targets.emplace_back(mapidx(*pos));
            } else {
                auto store = e.component<item_stored_t>();
                if (store != nullptr) {
                    auto storage_entity = entity(store->stored_in);
                    if (storage_entity) {
                        auto spos = storage_entity->component<position_t>();
                        targets.emplace_back(mapidx(*spos));
                    }
                }
            }
        });
        std::cout << "Updating pick map - " << targets.size() << " candidates.\n";
        pick_map.update(targets);
    }

    void update_harvest_map() {
        std::vector<int> targets;
        for (auto it = designations->harvest.begin(); it != designations->harvest.end(); ++it) {
            targets.emplace_back(mapidx(it->second));
        }
        harvest_map.update(targets);
    }
}

void distance_map_system::configure() {
    system_name = "Distance Maps";
    subscribe_mbox<huntable_moved_message>();
    subscribe_mbox<butcherable_moved_message>();
    subscribe_mbox<bed_changed_message>();
    subscribe_mbox<settler_moved_message>();
    subscribe_mbox<architecture_changed_message>();
    subscribe_mbox<blocks_changed_message>();
    subscribe_mbox<map_changed_message>();
    subscribe_mbox<leverpull_changed_message>();
    subscribe_mbox<axemap_changed_message>();
    subscribe_mbox<pickmap_changed_message>();
    subscribe_mbox<harvestmap_changed_message>();
}

void distance_map_system::update(const double duration_ms) {
    each_mbox<huntable_moved_message>([this] (const huntable_moved_message &msg) { update_huntables = true; });
    each_mbox<butcherable_moved_message>([this] (const butcherable_moved_message &msg) { update_butcherables = true; });
    each_mbox<bed_changed_message>([this] (const bed_changed_message &msg) { update_bed_map = true; });
    each_mbox<settler_moved_message>([this] (const settler_moved_message &msg) { update_settler_map = true; });
    each_mbox<architecture_changed_message>([this] (const architecture_changed_message &msg) { update_architecture_map = true; });
    each_mbox<blocks_changed_message>([this] (const blocks_changed_message &msg) { update_blocks_map = true; });
    each_mbox<leverpull_changed_message>([this] (const leverpull_changed_message &msg) { update_levers_map = true; });
    each_mbox<axemap_changed_message>([this] (const axemap_changed_message &msg) { update_axe_map = true; });
    each_mbox<pickmap_changed_message>([this] (const pickmap_changed_message &msg) { update_pick_map = true; });
    each_mbox<harvestmap_changed_message>([this] (const harvestmap_changed_message &msg) { update_harvest_map = true; });
    each_mbox<map_changed_message>([this] (const map_changed_message &msg) {
        update_huntables = true;
        update_butcherables = true;
        update_bed_map = true;
        update_settler_map = true;
        update_architecture_map = true;
        update_blocks_map = true;
        update_levers_map = true;
        update_axe_map = true;
        update_pick_map = true;
        update_harvest_map = true;
    });

    if (update_huntables) {
        dijkstra::update_hunting_map();
        update_huntables = false;
    }

    if (update_butcherables) {
        dijkstra::update_butcher_map();
        update_butcherables = false;
    }

    if (update_bed_map) {
        dijkstra::update_bed_map();
        update_bed_map = false;
    }

    if (update_settler_map) {
        dijkstra::update_settler_map();
        update_settler_map = false;
    }

    if (update_architecture_map) {
        dijkstra::update_architecure_map();
        update_architecture_map = false;
    }

    if (update_blocks_map) {
        dijkstra::update_blocks_map();
        update_blocks_map = false;
    }

    if (update_levers_map) {
        dijkstra::update_levers_map();
        update_levers_map = false;
    }

    if (update_axe_map) {
        dijkstra::update_axe_map();
        update_axe_map = false;
    }

    if (update_pick_map) {
        dijkstra::update_pick_map();
        update_pick_map = false;
    }

    if (update_harvest_map) {
        dijkstra::update_harvest_map();
        update_harvest_map = false;
    }
}
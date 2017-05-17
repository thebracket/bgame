#include "stockpile_system.hpp"
#include "../components/stockpile.hpp"
#include "../components/item.hpp"
#include "../../main/game_designations.hpp"
#include "../planet/region/region.hpp"

using namespace region;

namespace stockpile_sys {
    std::unordered_map<std::size_t, stockpile_info_t> stockpiles;
    std::unordered_map<int, std::vector<std::size_t>> stockpile_targets;
    std::vector<storable_item_t> storable_items;
}

using namespace stockpile_sys;

void stockpile_system::on_message(const tick_message &msg) {
    stockpiles.clear();
    stockpile_targets.clear();
    storable_items.clear();

    bool has_stockpiles = false;
    // Build a list of stockpiles and their targets
    each<stockpile_t>([&has_stockpiles] (entity_t &e, stockpile_t &sp) {
        has_stockpiles = true;

        stockpile_info_t spi{e.id, sp.category};
        stockpiles[e.id] = spi;
        for (int i=0; i<128; ++i) {
            if (sp.category.test(i)) {
                auto finder = stockpile_targets.find(e.id);
                if (finder == stockpile_targets.end()) {
                    stockpile_targets[i] = std::vector<std::size_t>{ e.id };
                } else {
                    stockpile_targets[i].push_back(e.id);
                }
            }
        }
    });
    if (!has_stockpiles) return;

    // Build a free tiles list
    for (int i=0; i<REGION_TILES_COUNT; ++i) {
        const int spid = stockpile_id(i);
        if (spid > 0) {
            ++stockpiles[spid].free_capacity;
            stockpiles[spid].open_tiles.insert(i);
        }
    }

    // Find items in stockpiles and items not in stockpiles
    each<item_t, position_t>([] (entity_t &e, item_t &i, position_t &pos) {
        const int idx = mapidx(pos);
        if (stockpile_id(idx) > 0) {
            // If it is already in a stockpile, reduce that capacity
            --stockpiles[stockpile_id(idx)].free_capacity;
            stockpiles[stockpile_id(idx)].open_tiles.erase(idx);
            return;
        }

        // If the item is claimed, do nothing
        if (i.claimed) return;

        // Determine the destination stockpile; Clothing is a special case
        int item_stockpile_idx = 0;
        if (i.type == CLOTHING) {
            item_stockpile_idx = clothing_stockpile;
        } else {
            auto finder = item_defs.find(i.item_tag);
            if (finder != item_defs.end()) {
                item_stockpile_idx = finder->second.stockpile_idx;
            }
        }

        // No stockpile for this? Escape!
        if (item_stockpile_idx == 0) return;

        // Look for capacity
        auto finder = stockpile_targets.find(item_stockpile_idx);
        if (finder == stockpile_targets.end()) return; // If there's nowhere interested, escape
        for (const auto &stockpile_id : finder->second) {
            if (stockpiles[stockpile_id].free_capacity > 0) {
                --stockpiles[stockpile_id].free_capacity;
                int destination_idx = *stockpiles[stockpile_id].open_tiles.begin();
                stockpiles[stockpile_id].open_tiles.erase(destination_idx);
                storable_items.push_back(storable_item_t{e.id, destination_idx});
                return; // Bail out since we've done this item
            }
        }
    });
}
#pragma once

#include <rltk.hpp>
#include "../../messages/messages.hpp"

struct stockpile_info_t {
    stockpile_info_t() {}
    stockpile_info_t(const std::size_t &ID, const std::bitset<128> &cat) : id(ID), category(cat) {}

    std::size_t id;
    std::bitset<128> category;
    int free_capacity = 0;
    std::unordered_set<int> open_tiles;
};

struct storable_item_t {
    storable_item_t() {}
    storable_item_t(const std::size_t &id, const int &dest) : item_id(id), dest_tile(dest) {}

    std::size_t item_id;
    int dest_tile;
};

namespace stockpile_sys {
    extern boost::container::flat_map<std::size_t, stockpile_info_t> stockpiles;
    extern boost::container::flat_map<int, std::vector<std::size_t>> stockpile_targets;
    extern std::vector<storable_item_t> storable_items;
}

class stockpile_system : public rltk::mailbox_system<tick_message> {
public:
    stockpile_system() { system_name = "Stockpiles"; }
    void on_message(const tick_message &msg);
};

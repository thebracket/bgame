#pragma once

#include "../components/items/item_carried.hpp"
#include "defs/item_def_t.hpp"
#include "../bengine/ecs_impl.hpp"

// Initializer
void load_raws();

// Item creation
void spawn_item_on_ground(const int x, const int y, const int z, const std::string &tag, const std::size_t &material, uint8_t quality=3, uint8_t wear=100, std::size_t creator_id=0, std::string creator_name="");
bengine::entity_t * spawn_item_on_ground_ret(const int x, const int y, const int z, const std::string &tag, const std::size_t &material, uint8_t quality = 3, uint8_t wear = 100, std::size_t creator_id = 0, std::string creator_name = "");
void spawn_item_in_container(const std::size_t container_id, const std::string &tag, const std::size_t &material, uint8_t quality = 3, uint8_t wear = 100, std::size_t creator_id = 0, std::string creator_name = "");
void spawn_item_carried(const std::size_t holder_id, const std::string &tag, const std::size_t &material, const item_location_t &loc, uint8_t quality = 3, uint8_t wear = 100, std::size_t creator_id = 0, std::string creator_name = "");
void decorate_item_categories(bengine::entity_t &item, std::bitset<NUMBER_OF_ITEM_CATEGORIES> &categories);

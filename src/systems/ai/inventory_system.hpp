#pragma once

#include <rltk.hpp>
#include "../raws/raws.hpp"
#include "../main/game_globals.hpp"
#include "../components/item.hpp"
#include "../components/building.hpp"
#include "../raws/reaction_input.hpp"

class inventory_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
	bool dirty = true;
};

int item_category_available(const int &category);
bool is_item_category_available(const int &category);
bool is_ammo_available(const std::string &ammo_type);
std::size_t claim_closest_item_by_category(const int &category, position_t &pos, const int range=-1);
std::size_t claim_closest_ammo(const int &category, position_t &pos, const std::string &ammo_type, const int range=-1);
position_t * get_item_location(std::size_t id);
std::size_t find_armor_upgrade(entity_t &entity, const int range=-1);

std::vector<available_building_t> get_available_buildings();
std::vector<std::pair<std::string, std::string>> get_available_reactions();

int available_items_by_tag(const std::string &tag);
int available_items_by_reaction_input(const reaction_input_t &input);
int blocks_available();
std::size_t claim_item_by_tag(const std::string &tag);
std::size_t claim_item_by_reaction_input(const reaction_input_t &input);
void unclaim_by_id(const std::size_t &id);

void delete_item(const std::size_t &id);
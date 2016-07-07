#pragma once

#include <rltk.hpp>
#include "../components/components.hpp"
#include "../raws/raws.hpp"
#include "../game_globals.hpp"

struct inventory_item_t {
	std::size_t id;
	bool claimed = false;
	boost::optional<position_t> pos;
	std::bitset<NUMBER_OF_ITEM_CATEGORIES> categories;
	std::string item_name;
	std::string item_tag;
};

class inventory_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
	bool dirty = true;
};

bool is_item_category_available(const int &category);
inventory_item_t claim_closest_item_by_category(const int &category, position_t &pos);
position_t get_item_location(std::size_t id);

std::vector<available_building_t> get_available_buildings();
std::vector<std::pair<std::string, std::string>> get_available_reactions();

int available_items_by_tag(const std::string &tag);
std::size_t claim_item_by_tag(const std::string &tag);
void unclaim_by_id(const std::size_t &id);

void delete_item(const std::size_t &id);
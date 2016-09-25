#pragma once

#include <rltk.hpp>
#include "../components/components.hpp"
#include "../raws/raws.hpp"
#include "../main/game_globals.hpp"
#include <boost/optional.hpp>

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
std::size_t claim_closest_item_by_category(const int &category, position_t &pos);
std::size_t claim_closest_ammo(const int &category, position_t &pos, const std::string &ammo_type);
boost::optional<position_t&> get_item_location(std::size_t id);
boost::optional<std::size_t> find_armor_upgrade(entity_t &entity);

std::vector<available_building_t> get_available_buildings();
std::vector<std::pair<std::string, std::string>> get_available_reactions();

int available_items_by_tag(const std::string &tag);
std::size_t claim_item_by_tag(const std::string &tag);
void unclaim_by_id(const std::size_t &id);

void delete_item(const std::size_t &id);
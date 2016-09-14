#include "weapons_helpers.hpp"
#include "../components/components.hpp"
#include "../messages/messages.hpp"

bool has_melee_weapon(const entity_t &entity) 
{
	bool has_weapon = false;
	each<item_carried_t>([&entity, &has_weapon] (entity_t &E, item_carried_t &item) {
		if (item.carried_by == entity.id && item.location == EQUIP_MELEE) has_weapon = true;
	});
	return has_weapon;
}

std::pair<bool, std::string> has_ranged_weapon(const entity_t &entity)
{
	bool has_weapon = false;
	std::string ammo_type = "";
	each<item_carried_t, item_t>([&entity, &has_weapon, &ammo_type] (entity_t &E, item_carried_t &item, item_t &i) {
		if (item.carried_by == entity.id && item.location == EQUIP_RANGED) {
			has_weapon = true;
			ammo_type = item_defs.find(i.item_tag)->second.ammo;
		}
	});
	return std::make_pair(has_weapon, ammo_type);
}

bool has_appropriate_ammo(const entity_t &entity, const std::string ammo_type, const position_t &pos)
{
	bool has_weapon = false;
	each<item_carried_t, item_t>([&entity, &has_weapon, &ammo_type, &pos] (entity_t &E, item_carried_t &item, item_t &i) {
		if (item.carried_by == entity.id && item.location == EQUIP_AMMO) { 
			if (item_defs.find(i.item_tag)->second.ammo == ammo_type) {
				has_weapon = true;
			} else {
				emit(drop_item_message{E.id, pos.x, pos.y, pos.z});
			}
		}
	});
	return has_weapon;
}

int shooting_range(const entity_t &entity, const position_t &pos)
{
	int result = -1;
	auto ranged_status = has_ranged_weapon(entity);
	if (ranged_status.first && has_appropriate_ammo(entity, ranged_status.second, pos)) {
		each<item_carried_t, item_t>([&entity, &result] (entity_t &E, item_carried_t &item, item_t &i) {
			if (item.carried_by == entity.id && item.location == EQUIP_RANGED) {
				result = item_defs.find(i.item_tag)->second.range;
			}
		});
	}
	return result;
}

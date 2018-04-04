#include "stdafx.h"
#include "weapons_helper.hpp"
#include "../../raws/items.hpp"
#include "../../raws/defs/item_def_t.hpp"
#include "../../raws/species.hpp"
#include "../../raws/defs/raw_creature_t.hpp"
#include "../../raws/materials.hpp"
#include "../../raws/defs/material_def_t.hpp"
#include "../../raws/creatures.hpp"
#include "../../raws/clothing.hpp"
#include "../../raws/defs/clothing_t.hpp"
#include "../ai/inventory_system.hpp"
#include "../../utils/system_log.hpp"

namespace weapons {

	using namespace bengine;

	bool has_melee_weapon(const entity_t &entity)
	{
		bool has_weapon = false;
		each<item_carried_t>([&entity, &has_weapon](entity_t &E, item_carried_t &item) {
			if (item.carried_by == entity.id && item.location == EQUIP_MELEE) has_weapon = true;
		});
		return has_weapon;
	}

	std::pair<bool, std::string> has_ranged_weapon(const entity_t &entity)
	{
		bool has_weapon = false;
		std::string ammo_type = "";
		each<item_carried_t, item_t>([&entity, &has_weapon, &ammo_type](entity_t &E, item_carried_t &item, item_t &i) {
			if (item.carried_by == entity.id && item.location == EQUIP_RANGED) {
				has_weapon = true;
				ammo_type = get_item_def(i.item_tag)->ammo;
			}
		});
		return std::make_pair(has_weapon, ammo_type);
	}

	bool has_appropriate_ammo(const entity_t &entity, const std::string ammo_type, const position_t &pos)
	{
		bool has_weapon = false;
		each<item_carried_t, item_t>([&entity, &has_weapon, &ammo_type, &pos](entity_t &E, item_carried_t &item, item_t &i) {
			if (item.carried_by == entity.id && item.location == EQUIP_AMMO) {
				if (get_item_def(i.item_tag)->ammo == ammo_type) {
					has_weapon = true;
				}
				else {
					systems::inventory_system::drop_item(E.id, pos.x, pos.y, pos.z );
				}
			}
		});
		return has_weapon;
	}

	int shooting_range(entity_t &entity, const position_t &pos)
	{
		int result = -1;
		auto ranged_status = has_ranged_weapon(entity);
		if (ranged_status.first && has_appropriate_ammo(entity, ranged_status.second, pos)) {
			each<item_carried_t, item_t>([&entity, &result](entity_t &E, item_carried_t &item, item_t &i) {
				if (item.carried_by == entity.id && item.location == EQUIP_RANGED) {
					result = get_item_def(i.item_tag)->range;
				}
			});
		}
		else {
			const auto na = entity.component<natural_attacks_t>();
			if (na) {
				for (const auto &a : na->attacks) {
					if (a.range > 0) result = a.range;
				}
			}
		}
		return result;
	}

	int calculate_armor_class(entity_t &entity) {
		float ac = 10.0;

		// Is it a creature?
		auto grazer = entity.component<grazer_ai>();
		if (grazer) {
			auto species = entity.component<species_t>();
			if (species) {
				auto finder = get_creature_def(species->tag);
				if (!finder) {
					glog(log_target::GAME, log_severity::warning, "WARNING: Unable to find species AC {0}", species->tag);
				}
				else {
					return finder->armor_class;
				}
			}
		}

		// Does it have any items that help?
		each<item_carried_t, item_t>([&ac, &entity](entity_t &e, item_carried_t &c, item_t &i) {
			if (c.carried_by == entity.id && c.location != INVENTORY && i.type == CLOTHING) {
				auto finder = get_clothing_by_tag(i.item_tag);
				if (finder) {
					ac += finder->armor_class + get_material(i.material)->ac_bonus;
				}
				auto quality = e.component<item_quality_t>();
				if (quality) {
					if (quality->quality > 3) ++ac;
					if (quality->quality == 7) ++ac; // Extra bonus for masterwork
				}
			}
		});

		return ac;
	}

	std::size_t get_melee_id(const entity_t &entity) {
		std::size_t weapon_id = 0;
		each<item_carried_t>([&entity, &weapon_id](entity_t &E, item_carried_t &item) {
			if (item.carried_by == entity.id && item.location == EQUIP_MELEE) weapon_id = E.id;
		});
		return weapon_id;
	}

	std::pair<std::size_t, std::size_t> get_ranged_and_ammo_id(const entity_t &entity) {
		std::size_t weapon_id = 0;
		std::size_t ammo_id = 0;
		each<item_carried_t>([&entity, &weapon_id, &ammo_id](entity_t &E, item_carried_t &item) {
			if (item.carried_by == entity.id && item.location == EQUIP_RANGED) weapon_id = E.id;
			if (item.carried_by == entity.id && item.location == EQUIP_AMMO) ammo_id = E.id;
		});
		return std::make_pair(weapon_id, ammo_id);
	}

	int get_weapon_initiative_penalty(const std::size_t &weapon_id) {
		if (weapon_id == 0) return 0;

		auto weapon_entity = entity(weapon_id);
		if (weapon_entity) {
			auto weapon_component = weapon_entity->component<item_t>();
			if (weapon_component) {
				auto finder = get_item_def(weapon_component->item_tag);
				if (finder != nullptr) {
					return finder->initiative_penalty;
				}
			}
		}
		return 0;
	}

}

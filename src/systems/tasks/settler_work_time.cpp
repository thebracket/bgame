#include "settler_work_time.hpp"
#include "work_types/equip_melee_work.hpp"
#include "work_types/equip_ranged_work.hpp"
#include "work_types/equip_armor_work.hpp"
#include "work_types/demolition_work.hpp"
#include "work_types/hauling_work.hpp"
#include "../ai/inventory_system.hpp"
#include "../damage/weapons_helpers.hpp"
#include "idle_mode.hpp"
#include "settler_glyph.hpp"
#include "settler_job_status.hpp"
#include "settler_drop_tool.hpp"
#include "settler_cancel_action.hpp"
#include "pathfinding.hpp"
#include "initiative.hpp"
#include "settler_wander.hpp"
#include "../stockpile_system.hpp"
#include "../../main/game_designations.hpp"
#include "../../raws/defs/item_def_t.hpp"

using namespace rltk;
using tasks::become_idle;
using tasks::change_settler_glyph;
using tasks::change_job_status;
using tasks::drop_current_tool;
using tasks::cancel_action;
using tasks::follow_path;
using tasks::follow_result_t;
using tasks::calculate_initiative;

void do_work_time(entity_t &entity, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.job_type_major == JOB_IDLE) {
		// Find something to do!
		const auto idx = mapidx(pos.x, pos.y, pos.z);

		if (ai.permitted_work[JOB_CONSTRUCTION] && designations->deconstructions.size() > 0) {
			unbuild_t building = designations->deconstructions.back();
			designations->deconstructions.pop_back();
			if (building.is_building) {
				ai.target_id = building.building_id;
				change_settler_glyph(entity, vchar{1, rltk::colors::Pink, rltk::colors::BLACK});
				ai.job_type_major = JOB_DECONSTRUCT;
				ai.job_type_minor = JM_FIND_DECONSTRUCT;
				change_job_status(ai, name, "performing demolition.", true);
			} else {
				std::cout << building.building_id << "\n";
				ai.target_id = building.building_id;
				change_settler_glyph(entity, vchar{1, rltk::colors::RED, rltk::colors::BLACK});
				ai.job_type_major = JOB_DEMOLISH;
				ai.job_type_minor = JM_FIND_DEMOLISH;
				change_job_status(ai, name, "performing structural demolition.", true);
			}
			return;
		}

		// If we don't have a ranged weapon, and one is available, equip it
		std::pair<bool, std::string> ranged_status = has_ranged_weapon(entity);
		if (is_item_category_available(WEAPON_RANGED) && ranged_status.first==false) {
			change_settler_glyph(entity, vchar{1, rltk::colors::WHITE, rltk::colors::BLACK});
			ai.job_type_major = JOB_EQUIP_RANGED;
			ai.job_type_minor = JM_FIND_RANGED_WEAPON;
			change_job_status(ai, name, "Finding a ranged weapon.", false);
			return;
		}

		// Likewise, search for ammo if available
		bool has_ammo = has_appropriate_ammo(entity, ranged_status.second, pos);
		if (designations->standing_order_upgrade > standing_orders::SO_UPGRADE_NEVER && ranged_status.first && !has_ammo && is_ammo_available(ranged_status.second)) {
			change_settler_glyph(entity, vchar{1, rltk::colors::WHITE, rltk::colors::BLACK});
			ai.job_type_major = JOB_EQUIP_AMMO;
			ai.job_type_minor = JM_FIND_AMMO;
			change_job_status(ai, name, "Finding ammunition.");
			return;
		}

		// If we don't have a melee weapon, and one is available, equip it
		// TODO: This needs fixing - it gets stuck, commented out for now.
		/*if (designations->standing_order_upgrade > standing_orders::SO_UPGRADE_NEVER && is_item_category_available(WEAPON_MELEE) && has_melee_weapon(entity)==false) {
			change_settler_glyph(entity, vchar{1, rltk::colors::WHITE, rltk::colors::BLACK});
			ai.job_type_major = JOB_EQUIP_MELEE;
			ai.job_type_minor = JM_FIND_MELEE_WEAPON;
			change_job_status(ai, name, "Finding a melee weapon.", false);
			return;
		}*/

		// Look for improved armor
		if (designations->standing_order_upgrade > standing_orders::SO_UPGRADE_NEVER) {
			int max_range = -1;
			if (designations->standing_order_upgrade == standing_orders::SO_UPGRADE_NEARBY) max_range = 15;
			std::size_t better_armor = find_armor_upgrade(entity, max_range);
			if (better_armor>0) {
				change_settler_glyph(entity, vchar{1, rltk::colors::WHITE, rltk::colors::BLACK});
				ai.job_type_major = JOB_EQUIP_ARMOR;
				ai.job_type_minor = JM_FIND_ARMOR;
				ai.target_id = better_armor;
				change_job_status(ai, name, "Finding armor.", true);
				return;
			}
		}

		// Look for stockpile/tidy tasks
        if (ai.permitted_work[JOB_HAULING] && !stockpile_sys::storable_items.empty()) {
            ai.job_type_major = JOB_TIDY;
            ai.job_type_minor = JM_FIND_TIDY;
            storable_item_t item = stockpile_sys::storable_items[0];
            stockpile_sys::storable_items.erase(stockpile_sys::storable_items.begin(), stockpile_sys::storable_items.begin()+1);
            ai.target_id = item.item_id;
            std::tie(ai.target_x, ai.target_y, ai.target_z) = idxmap(item.dest_tile);
        }

	} else if (ai.job_type_major == JOB_EQUIP_MELEE) {
		do_equip_melee(entity, ai, stats, species, pos, name);
		return;
	} else if (ai.job_type_major == JOB_EQUIP_RANGED) {
		do_equip_ranged(entity, ai, stats, species, pos, name);
		return;
	} else if (ai.job_type_major == JOB_EQUIP_AMMO) {
		do_equip_ammo(entity, ai, stats, species, pos, name);
		return;
	} else if (ai.job_type_major == JOB_EQUIP_ARMOR) {
		do_equip_armor(entity, ai, stats, species, pos, name);
		return;
	} else if (ai.job_type_major == JOB_DECONSTRUCT) {
		do_deconstruction(entity, ai, stats, species, pos, name);
		return;
	} else if (ai.job_type_major == JOB_DEMOLISH) {
		do_demolition(entity, ai, stats, species, pos, name);
		return;
	} else if (ai.job_type_major == JOB_TIDY) {
        do_hauling(entity, ai, stats, species, pos, name);
        return;
    }
	wander_randomly(entity, pos);
}

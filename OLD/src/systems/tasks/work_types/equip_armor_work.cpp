#include "equip_armor_work.hpp"
#include "../../../messages/messages.hpp"
#include "../../ai/path_finding.hpp"
#include "../../ai/inventory_system.hpp"
#include "../idle_mode.hpp"
#include "../settler_glyph.hpp"
#include "../settler_job_status.hpp"
#include "../settler_drop_tool.hpp"
#include "../settler_cancel_action.hpp"
#include "../pathfinding.hpp"
#include "../initiative.hpp"
#include "../../../raws/clothing.hpp"
#include "../../../raws/defs/clothing_t.hpp"

using namespace rltk;
using tasks::become_idle;
using tasks::change_settler_glyph;
using tasks::change_job_status;
using tasks::drop_current_tool;
using tasks::cancel_action;
using tasks::follow_path;
using tasks::follow_result_t;
using tasks::calculate_initiative;

void do_equip_armor(entity_t &e, settler_ai_t &ai, game_stats_t &stats, species_t &species, position_t &pos, name_t &name) {
	if (ai.job_type_minor == JM_FIND_ARMOR) {
		auto axe_pos = get_item_location(ai.target_id);
		ai.current_path = find_path(pos, *axe_pos);
		if (!ai.current_path->success) {
			cancel_action(e, ai, stats, species, pos, name, "No route to available armor");
			return;
		}
		ai.job_type_minor = JM_GO_TO_ARMOR;
		ai.target_x = axe_pos->x;
		ai.target_y = axe_pos->y;
		ai.target_z = axe_pos->z;
		change_job_status(ai, name, "Traveling to armor");
		return;
	}

	if (ai.job_type_minor == JM_GO_TO_ARMOR) {
		tasks::try_path(e, ai, pos,
			[] () {}, // Do nothing on success
			[&ai, &name] () {
				ai.current_path.reset();
				ai.job_type_minor = JM_COLLECT_ARMOR;
				change_job_status(ai, name, "Collect armor");
			}, // On arrival
			[&e, &ai, &stats, &species, &pos, &name] () {
				unclaim_by_id(ai.current_tool);
				cancel_action(e, ai, stats, species, pos, name, "No route to armor");
			}
		);
		return;
	}

	if (ai.job_type_minor == JM_COLLECT_ARMOR) {
		// Find the pick, remove any position or stored components, add a carried_by component
		auto armor = entity(ai.target_id);
		if (armor) {
			auto item = armor->component<item_t>();
			item_location_t loc = INVENTORY;
			auto finder = get_clothing_by_tag(item->item_tag);
			if (finder->slot == "head") loc = HEAD;
			if (finder->slot == "torso") loc = TORSO;
			if (finder->slot == "legs") loc = LEGS;
			if (finder->slot == "shoes") loc = FEET;
			each<item_carried_t>([&e, &pos, &loc](entity_t &E, item_carried_t &c) {
				if (c.carried_by == e.id && c.location == loc) emit(drop_item_message{ E.id, pos.x, pos.y, pos.z });
			});
			emit(pickup_item_message{ static_cast<std::size_t>(ai.target_id), e.id, loc });
			ai.current_tool = 0;
		}

		become_idle(e, ai, name);
		return;
	}	
}

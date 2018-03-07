#pragma once

#include "../../../bengine/ecs.hpp"
#include "../../../components/ai_tags/ai_tag_my_turn.hpp"
#include "../../../components/position.hpp"
#include "../../physics/movement_system.hpp"
#include "../../../components/ai_tags/ai_mode_idle.hpp"
#include "../distance_map_system.hpp"
#include "../../../components/items/item.hpp"
#include "../../../components/settler_ai.hpp"
#include "../../../components/game_stats.hpp"
#include "../../../raws/raws.hpp"
#include "../../../raws/items.hpp"
#include "../../../raws/materials.hpp"
#include "../../../raws/defs/material_def_t.hpp"
#include "../../../raws/defs/item_def_t.hpp"
#include "../../../global_assets/rng.hpp"
#include "../../helpers/pathfinding.hpp"
#include "../../../components/items/item_stored.hpp"
#include "../../../components/items/item_carried.hpp"
#include "../inventory_system.hpp"
#include "../../../planet/region/region.hpp"
#include "../../physics/movement_system.hpp"

template<typename TAG>
class ai_work_template {
public:
	ai_work_template() { }

	inline void set_status(bengine::entity_t &e, const std::string &&status) {
		auto ai = e.component<settler_ai_t>();
		if (ai) {
			ai->job_status = status;
		}
	}

	template <typename F>
	void do_ai(const char * new_status, const F &&f)
	{
		bengine::each<TAG, ai_tag_my_turn_t, position_t, settler_ai_t>([&f, &new_status](bengine::entity_t &e, TAG &tag, ai_tag_my_turn_t &turn, position_t &pos, settler_ai_t &ai) {
			bengine::delete_component<ai_tag_my_turn_t>(e.id); // It's not my turn anymore

			ai.job_status = std::string(new_status);
			f(e, tag, turn, pos);

			// If not tagged for this work type, go idle
			if (e.component<TAG>() == nullptr) {
				e.assign(ai_mode_idle_t{});
			}
		});
	}

	void move_to(const int &entity_id, const position_t &pos, const position_t &destination) const {
		systems::movement::move_to(*bengine::entity(entity_id), pos, destination);
		//emit_deferred(entity_wants_to_move_message{ entity_id, destination });
	}

	template <typename CANCEL, typename SUCCESS>
	void folllow_path(systems::dijkstra::dijkstra_map &map, position_t &pos, bengine::entity_t &e, const CANCEL &cancel, const SUCCESS &success) const {
		const auto d = map.get(mapidx(pos));
		if (d > systems::dijkstra::MAX_DIJSTRA_DISTANCE - 1) {
			// We can't get to it
			cancel();
		}
		else if (d < 1) {
			// We're here!
			success();
		}
		else {
			// Path towards it
			position_t destination = map.find_destination(pos);
			move_to(e.id, pos, destination);
		}
	}

	template < typename ITEM_TYPE, typename CANCEL, typename SUCCESS >
	void pickup_tool(const bengine::entity_t &e, position_t &pos, std::size_t &out_tool, const CANCEL &cancel, const SUCCESS &success) const {
		std::size_t tool_id = 0;
		bengine::each<item_t, ITEM_TYPE>([&tool_id, &pos](bengine::entity_t &tool, item_t &item, ITEM_TYPE &t) {
			auto tool_pos = tool.component<position_t>();
			if (tool_pos != nullptr && *tool_pos == pos) {
				tool_id = tool.id;
			}
			else {
				auto stored = tool.component<item_stored_t>();
				if (stored != nullptr) {
					auto spos = bengine::entity(stored->stored_in)->component<position_t>();
					if (spos != nullptr && *spos == pos) {
						tool_id = tool.id;
					}
				}
			}
		});
		if (tool_id > 0) {
			out_tool = tool_id;
			systems::inventory_system::pickup_item(out_tool, e.id );
			//emit(MSG{});
			success();
		}
		else {
			// We've failed to get the tool!
			cancel();
		}
	}

	template <typename CANCEL, typename ARRIVED>
	void follow_path(TAG &tag, position_t &pos, bengine::entity_t &e, const CANCEL &&cancel, const ARRIVED &&arrived) {
		if (!tag.current_path || tag.current_path->success == false) {
			cancel();
			return;
		}
		if (pos == tag.current_path->destination || tag.current_path->steps.empty()) {
			arrived();
			return;
		}

		const position_t next_step = tag.current_path->steps.front();
		tag.current_path->steps.pop_front();
		if (next_step.x > 0 && next_step.x < REGION_WIDTH && next_step.y > 0 &&
			next_step.y < REGION_HEIGHT && next_step.z > 0 && next_step.z < REGION_DEPTH
			&& region::flag(mapidx(next_step), tile_flags::CAN_STAND_HERE))
		{
			move_to(e.id, pos, next_step);
		}
		else {
			// We couldn't get there
			tag.current_path.reset();
			cancel();
		}
	}

	inline void cancel_work_tag(bengine::entity_t &e) {
		bengine::delete_component<TAG>(e.id);
		set_status(e, "Idle");
	}
private:
};

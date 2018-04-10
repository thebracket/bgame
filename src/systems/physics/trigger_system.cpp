#include "stdafx.h"
#include "trigger_system.hpp"
#include "../../utils/thread_safe_message_queue.hpp"
#include "../../planet/region/region.hpp"
#include "../../global_assets/game_designations.hpp"
#include "../../global_assets/game_pause.hpp"
#include "../../global_assets/game_mode.hpp"
#include "../../raws/raws.hpp"
#include "../../raws/materials.hpp"
#include "../../raws/defs/material_def_t.hpp"
#include "../gui/log_system.hpp"
#include "../../global_assets/spatial_db.hpp"
#include "movement_system.hpp"
#include "../../bengine/IconsFontAwesome.h"
#include "../../render_engine/chunks/chunks.hpp"
#include "door_system.hpp"
#include "../damage/damage_system.hpp"
#include "../../global_assets/rng.hpp"
#include "systems/gui/circuit_graph.hpp"
#include "gravity_system.hpp"

using namespace bengine;

namespace systems {
	namespace triggers {

		using namespace bengine;
		using namespace region;
		using namespace logging;

		bool dirty = true;
		std::unordered_map<int, std::size_t> triggers;
		int trigger_id = 0;
		int selected_existing_link = 0;
		int new_link = 0;
		const std::string win_trigger_mgmt = std::string(ICON_FA_COG) + " Trigger Management";
		const std::string btn_remove_link = std::string(ICON_FA_MINUS) + " Remove Link";
		const std::string btn_add_link = std::string(ICON_FA_PLUS) + " Add Link";
		const std::string btn_close = std::string(ICON_FA_TIMES) + " Close";	
		static bool dependencies_changed = true;
		static std::set<int> nodes_changed;

		thread_safe_message_queue<triggers_changed_message> triggers_changed;
		thread_safe_message_queue<request_lever_pull_message> lever_pull_requests;
		thread_safe_message_queue<leverpull_changed_message> lever_pull_changes;
		thread_safe_message_queue<trigger_details_requested> trigger_details;
		thread_safe_message_queue<lever_pulled_message> lever_pulled;

		static void manage_triggers() {
			triggers_changed.process_all([](triggers_changed_message &msg) {
				//std::cout << "Received trigger notification\n";
				dirty = true;
			});

			if (dirty) {
				//std::cout << "Rebuilding trigger list\n";
				triggers.clear();
				each<entry_trigger_t, position_t>([](entity_t &e, entry_trigger_t &trigger, position_t &pos) {
					//std::cout << "Found a trigger!\n";
					triggers.insert(std::make_pair(mapidx(pos), e.id));
				});
				dirty = false;
			}
		}

		static void pull_levers() {
			lever_pull_requests.process_all([](request_lever_pull_message &msg) {
				designations->levers_to_pull.emplace_back(msg.lever_id);
				lever_pull_changes.enqueue(leverpull_changed_message{});
			});
		}

		static bool show_window = true;
		static int node_graph_id = -1;

		void edit_triggers() {
			trigger_details.process_all([](const trigger_details_requested &msg) {
				trigger_id = msg.lever_id;
				pause_mode = PAUSED;
				game_master_mode = TRIGGER_MANAGEMENT;
			});

			if (game_master_mode != TRIGGER_MANAGEMENT) return;

			if (node_graph_id != trigger_id)
			{
				node_graph_id = trigger_id;
				circuit_graph::build_nodes_list();
			}

			circuit_graph::display_circuit_graph(&show_window);
			if (!show_window) {
				circuit_graph::restore_state();
				game_master_mode = PLAY;
				show_window = true;
				node_graph_id = -1;
				circuit_graph::sync_node_list_to_ecs();
				dependencies_changed = true;
			}
		}

		static void cage_trap_fire(const systems::movement::entity_moved_message &msg, entity_t  * trigger_entity, const int &tile_index)
		{
			auto target_entity = entity(msg.entity_id);
			if (target_entity) {
				const auto grazer = target_entity->component<grazer_ai>();
				const auto sentient = target_entity->component<sentient_ai>();

				if (grazer || (sentient && sentient->hostile))
				{
					const auto name = target_entity->component<name_t>();
					if (name) {
						LOG ss;
						ss.other_name(msg.entity_id)->text(" is trapped in a cage!");
						logging::log_message msg_{ ss.chars };
						logging::log(msg_);
					}

					// TODO: Add a random chance with some dex involved
					// Spawn a cage object
					auto building = trigger_entity->component<building_t>();
					const auto material = get_material_by_tag(building->built_with[0].first);
					const auto &[x, y, z] = idxmap(tile_index);
					const auto new_cage = spawn_item_on_ground_ret(x, y, z, "cage", material, 3, 100);

					// Add a stored component
					target_entity->assign(item_stored_t{ new_cage->id });

					// Remove the position component
					delete_component<position_t>(msg.entity_id);

					// Remove the trap
					delete_entity(trigger_entity->id);
					triggers_changed.enqueue(triggers_changed_message{});
				}
			}
		}

		static void stonefall_trap_fire(const systems::movement::entity_moved_message &msg, entity_t  * trigger_entity, const int &tile_index)
		{
			auto target_entity = entity(msg.entity_id);
			if (target_entity) {
				const auto grazer = target_entity->component<grazer_ai>();
				const auto sentient = target_entity->component<sentient_ai>();
				if (grazer || (sentient && sentient->hostile))
				{
					const auto name = target_entity->component<name_t>();
					if (name) {
						LOG ss;
						ss.other_name(msg.entity_id)->text(" is hit by a falling rock trap!");
						logging::log_message msg_{ ss.chars };
						logging::log(msg_);
					}

					// Spawn some damage!
					damage_system::inflict_damage(damage_system::inflict_damage_message{ msg.entity_id, rng.roll_dice(3, 6), "falling rocks" });

					// Remove the trap
					delete_entity(trigger_entity->id);
					triggers_changed.enqueue(triggers_changed_message{});
				}
			}
		}

		static void blade_trap_fire(const systems::movement::entity_moved_message &msg, entity_t  * trigger_entity, const int &tile_index)
		{
			auto target_entity = entity(msg.entity_id);
			if (target_entity) {
				const auto grazer = target_entity->component<grazer_ai>();
				const auto sentient = target_entity->component<sentient_ai>();
				if (grazer || (sentient && sentient->hostile))
				{
					const auto name = target_entity->component<name_t>();
					if (name) {
						LOG ss;
						ss.other_name(msg.entity_id)->text(" is hit by a blade trap!");
						logging::log_message msg_{ ss.chars };
						logging::log(msg_);
					}

					// TODO: Add a random chance with some dex involved
					// Spawn some damage!
					damage_system::inflict_damage(damage_system::inflict_damage_message{ msg.entity_id, rng.roll_dice(3,8), "spinning blades" });

					// Extend the blades
					const auto building = target_entity->component<building_t>();
					if (building)
					{
						building->vox_model = 132;
					}
				}
			}
		}

		void entry_trigger_firing(const systems::movement::entity_moved_message &msg) {
			const auto tile_index = mapidx(msg.destination);
			const auto finder = triggers.find(tile_index);
			if (finder != triggers.end()) {
				const auto trigger_entity = entity(finder->second);
				if (trigger_entity) {
					const auto trigger_def = trigger_entity->component<entry_trigger_t>();
					if (trigger_def)
					{
						switch (trigger_def->type)
						{
						case TRIGGER_CAGE: cage_trap_fire(msg, trigger_entity, tile_index); break;
						case TRIGGER_STONEFALL: {} stonefall_trap_fire(msg, trigger_entity, tile_index); break;
						case TRIGGER_BLADE: {} blade_trap_fire(msg, trigger_entity, tile_index); break;
						case TRIGGER_PRESSURE: { trigger_def->active = !trigger_def->active; nodes_changed.insert(trigger_entity->id); } break;
						}
						trigger_def->active = true;
					}
				}
			}
		}

		static void pulled_levers() {
			lever_pulled.process_all([](const lever_pulled_message &msg) {
				auto lever_entity = entity(msg.lever_id);
				if (!lever_entity) return;
				auto lever_component = lever_entity->component<lever_t>();
				if (!lever_component) return;
				auto lever_building = lever_entity->component<building_t>();
				auto sender = lever_entity->component<sends_signal_t>();
				nodes_changed.insert(msg.lever_id);

				sender->active = !sender->active;
				lever_component->active = sender->active;
				if (lever_building) {
					if (lever_component->active) {
						lever_building->vox_model = 124;
					}
					else {
						lever_building->vox_model = 125;
					}
				}
			});
		}

		static void oscillators()
		{
			each<oscillator_t>([] (entity_t &e, oscillator_t &o)
			{
				o.ticker--;
				if (o.ticker == 0)
				{
					o.ticker = o.interval;
					o.active = !o.active;
					auto sender = e.component<sends_signal_t>();
					sender->active = o.active;
					nodes_changed.insert(e.id);
				}
			});
		}

		static void float_sensors()
		{
			each<float_gauge_t, position_t, sends_signal_t>([] (entity_t &e, float_gauge_t &f, position_t &pos, sends_signal_t &sender)
			{
				const auto idx = mapidx(pos);
				const auto water_level = region::water_level(idx);
				const auto old_status = sender.active;
				if (water_level >= f.water_level)
				{
					sender.active = true;
				} else
				{
					sender.active = false;
				}
				if (sender.active != old_status)
				{
					f.active = sender.active;
					nodes_changed.insert(e.id);
				}
			});
		}

		static void proximity_sensors()
		{
			each<proximity_sensor_t, position_t, viewshed_t, sends_signal_t>([] (entity_t &e, proximity_sensor_t &p, position_t &pos, viewshed_t &view, sends_signal_t &sender)
			{
				const auto old_state = sender.active;
				sender.active = !view.visible_entities.empty();
				if (sender.active)
				{
					sender.active = false;
					for (const auto &id : view.visible_entities)
					{
						const auto detected_e = entity(id);
						if (detected_e)
						{
							if (detected_e->component<settler_ai_t>() || detected_e->component<grazer_ai>() || detected_e->component<sentient_ai>()) sender.active = true;
						}
					}
				}
				if (old_state != sender.active)
				{
					p.active = sender.active;
					nodes_changed.insert(e.id);
				}
			});
		}

		static std::map<int, std::vector<int>> affects;

		static void calc_dependency()
		{
			dependencies_changed = false;
			affects.clear();
			each<receives_signal_t>([] (entity_t &e, receives_signal_t &s)
			{
				for (const auto &r : s.receives_from)
				{
					affects[std::get<0>(r)].emplace_back(e.id);
				}
			});
		}

		static void and_gate(entity_t * circuit_entity, signal_processor_t * cp)
		{
			auto all_set = true;
			const auto receiver = circuit_entity->component<receives_signal_t>();
			for (const auto &n : receiver->receives_from)
			{
				const auto r = entity(std::get<0>(n));
				const auto s = r->component<sends_signal_t>();
				if (!s->active) all_set = false;
			}
			cp->active = all_set;
			const auto sender = circuit_entity->component<sends_signal_t>();
			sender->active = cp->active;
			std::cout << "AND gate state: " << sender->active << "\n";
			nodes_changed.insert(circuit_entity->id);
		}

		static void or_gate(entity_t * circuit_entity, signal_processor_t * cp)
		{
			auto any_set = true;
			const auto receiver = circuit_entity->component<receives_signal_t>();
			for (const auto &n : receiver->receives_from)
			{
				const auto r = entity(std::get<0>(n));
				const auto s = r->component<sends_signal_t>();
				if (s->active) any_set = true;
			}
			cp->active = any_set;
			const auto sender = circuit_entity->component<sends_signal_t>();
			sender->active = cp->active;
			nodes_changed.insert(circuit_entity->id);
		}

		static void not_gate(entity_t * circuit_entity, signal_processor_t * cp)
		{
			auto all_set = true;
			const auto receiver = circuit_entity->component<receives_signal_t>();
			for (const auto &n : receiver->receives_from)
			{
				const auto r = entity(std::get<0>(n));
				const auto s = r->component<sends_signal_t>();
				if (!s->active) all_set = false;
			}
			all_set = !all_set;
			cp->active = all_set;
			const auto sender = circuit_entity->component<sends_signal_t>();
			sender->active = cp->active;
			nodes_changed.insert(circuit_entity->id);
		}

		static void nand_gate(entity_t * circuit_entity, signal_processor_t * cp)
		{
			auto all_set = true;
			const auto receiver = circuit_entity->component<receives_signal_t>();
			for (const auto &n : receiver->receives_from)
			{
				const auto r = entity(std::get<0>(n));
				const auto s = r->component<sends_signal_t>();
				if (!s->active) all_set = false;
			}
			all_set = !all_set;
			cp->active = all_set;
			const auto sender = circuit_entity->component<sends_signal_t>();
			sender->active = cp->active;
			nodes_changed.insert(circuit_entity->id);
		}

		static void nor_gate(entity_t * circuit_entity, signal_processor_t * cp)
		{
			auto any_set = true;
			const auto receiver = circuit_entity->component<receives_signal_t>();
			for (const auto &n : receiver->receives_from)
			{
				const auto r = entity(std::get<0>(n));
				const auto s = r->component<sends_signal_t>();
				if (s->active) any_set = true;
			}
			cp->active = !any_set;
			const auto sender = circuit_entity->component<sends_signal_t>();
			sender->active = cp->active;
			nodes_changed.insert(circuit_entity->id);
		}

		static void xor_gate(entity_t * circuit_entity, signal_processor_t * cp)
		{
			auto set_count = 0;
			const auto receiver = circuit_entity->component<receives_signal_t>();
			for (const auto &n : receiver->receives_from)
			{
				const auto r = entity(std::get<0>(n));
				const auto s = r->component<sends_signal_t>();
				if (s->active) ++set_count;
			}
			cp->active = (set_count == 0 || set_count == receiver->receives_from.size());
			const auto sender = circuit_entity->component<sends_signal_t>();
			sender->active = cp->active;
			nodes_changed.insert(circuit_entity->id);
		}

		static void evaluate_signal_processor(entity_t * circuit_entity, signal_processor_t * cp)
		{
			// TODO: Processing

			switch (cp->processor)
			{
			case AND: and_gate(circuit_entity, cp); break;
			case OR:  or_gate(circuit_entity, cp);  break;
			case NOTGATE: not_gate(circuit_entity, cp); break;
			case NAND: nand_gate(circuit_entity, cp); break;
			case NOR:  nor_gate(circuit_entity, cp); break;
			case XOR: xor_gate(circuit_entity, cp); break;
			}
			nodes_changed.insert(circuit_entity->id);
		}

		static void evaluate_circuit_node(const int &id, const int &sender)
		{
			auto circuit_entity = entity(id);
			if (!circuit_entity) return;
			const auto cp = circuit_entity->component<signal_processor_t>();
			if (cp)
			{
				// Evaluate signal propagation
				evaluate_signal_processor(circuit_entity, cp);
			}
			const auto door = circuit_entity->component<construct_door_t>();
			if (door)
			{
				// Evaluate door
				const auto sender_e = entity(sender);
				const auto sender_s = sender_e->component<sends_signal_t>();
				door->locked = sender_s->active;
				doors::doors_changed();
			}
			const auto bridge = circuit_entity->component<bridge_t>();
			if (bridge)
			{
				// Evaluate bridge
				const auto sender_e = entity(sender);
				const auto sender_s = sender_e->component<sends_signal_t>();
				bridge->retracted = sender_s->active;
				if (bridge->retracted) {
					// Retract the bridge
					for (auto i = 0; i<REGION_TILES_COUNT; ++i) {
						if (bridge_id(i) == id) {
							make_open_space(i);
							chunks::mark_chunk_dirty_by_tileidx(i);
						}
					}
				}
				else {
					// Extend the bridge!
					for (int i = 0; i<REGION_TILES_COUNT; ++i) {
						if (bridge_id(i) == id) {
							make_floor(i);
							chunks::mark_chunk_dirty_by_tileidx(i);
						}
					}
				}
				tile_recalc_all();
				gravity::tile_was_removed();
			}
			const auto building = circuit_entity->component<building_t>();
			if (building && building->tag == "spike_trap")
			{
				// Evaluate building; currently spike traps
				const auto sender_e = entity(sender);
				const auto sender_s = sender_e->component<sends_signal_t>();

				const auto target_pos = circuit_entity->component<position_t>();
				const auto receiver = circuit_entity->component<receives_signal_t>();

				if (target_pos && receiver) {
					if (sender_s->active) {
						building->vox_model = 130;
						// Attack everything in the tile
						const auto &[x, y, z] = idxmap(mapidx(*target_pos));
						auto visible_here = entity_octree.find_by_loc(octree_location_t{ x, y, z, 0 });
						for (const auto &v : visible_here) {
							auto victim_entity = entity(v);
							if (victim_entity) {
								const auto health = victim_entity->component<health_t>();
								if (health) {
									systems::damage_system::inflict_damage(systems::damage_system::inflict_damage_message{ v, rng.roll_dice(2,8), "Floor Spikes" });
								}
							}
						}
					}
					else {
						building->vox_model = 129;
					}
				}
			} else if (building && building->tag == "support")
			{
				const auto rs = circuit_entity->component<receives_signal_t>();
				for (auto &sender_id : rs->receives_from)
				{
					const auto tmp_e = entity(std::get<0>(sender_id));
					if (tmp_e)
					{
						const auto ss = tmp_e->component<sends_signal_t>();
						if (ss)
						{
							ss->targets.erase(
								std::remove_if(ss->targets.begin(), ss->targets.end(), [&sender_id](const auto &a) { return a == std::get<0>(sender_id); }),
								ss->targets.end()
							);
						}
					}
				}
				delete_entity(circuit_entity->id);
				gravity::tile_was_removed();
			}
		}

		static void run_circuits()
		{
			while (!nodes_changed.empty())
			{
				const auto id = *nodes_changed.begin();
				std::cout << id << "\n";
				nodes_changed.erase(id);

				const auto effect_finder = affects.find(id);
				if (effect_finder != affects.end())
				{
					for (const auto affected : effect_finder->second)
					{
						evaluate_circuit_node(affected, id);
					}
				}
			}
		}

		void run(const double &duration_ms) {
			// Reset all pressure plates
			each<pressure_plate_t, sends_signal_t>([] (entity_t &e, pressure_plate_t &p, sends_signal_t &s)
			{
				s.active = false;
			});

			manage_triggers();
			pull_levers();
			pulled_levers();
			oscillators();
			float_sensors();
			if (slow_tick) proximity_sensors();
			if (dependencies_changed) calc_dependency();
			run_circuits();
		}
	}
}
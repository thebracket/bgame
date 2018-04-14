#pragma once

#include "farming/farm_designs.hpp"
#include "ai_tags/ai_tag_work_farm_plant.hpp"
#include "ai_tags/ai_tag_work_guarding.hpp"
#include "ai_tags/ai_mode_idle.hpp"
#include "ai_tags/ai_settler_new_arrival.hpp"
#include "ai_tags/ai_tag_leisure_shift.hpp"
#include "ai_tags/ai_tag_my_turn.hpp"
#include "ai_tags/ai_tag_sleep_shift.hpp"
#include "ai_tags/ai_tag_work_architect.hpp"
#include "ai_tags/ai_tag_work_building.hpp"
#include "ai_tags/ai_tag_work_butcher.hpp"
#include "ai_tags/ai_tag_work_farm_clear.hpp"
#include "ai_tags/ai_tag_work_farm_fertilize.hpp"
#include "ai_tags/ai_tag_work_farm_fixsoil.hpp"
#include "ai_tags/ai_tag_work_farm_water.hpp"
#include "ai_tags/ai_tag_work_farm_weed.hpp"
#include "ai_tags/ai_tag_work_harvest.hpp"
#include "ai_tags/ai_tag_work_hunting.hpp"
#include "ai_tags/ai_tag_work_lumberjack.hpp"
#include "ai_tags/ai_tag_work_mining.hpp"
#include "ai_tags/ai_tag_work_order.hpp"
#include "ai_tags/ai_tag_work_pull_lever.hpp"
#include "ai_tags/ai_tag_work_shift.hpp"
#include "ai_tags/ai_tag_work_stockpiles.hpp"
#include "buildings/architecture_designations_t.hpp"
#include "buildings/bridge.hpp"
#include "buildings/building.hpp"
#include "buildings/building_designations_t.hpp"
#include "buildings/construct_container.hpp"
#include "buildings/construct_power.hpp"
#include "buildings/construct_provides_door.hpp"
#include "buildings/construct_provides_sleep.hpp"
#include "machinery/entry_trigger.hpp"
#include "machinery/receives_signal.hpp"
#include "buildings/smoke_emitter.hpp"
#include "buildings/turret_t.hpp"
#include "farming/designated_farmer.hpp"
#include "farming/designated_hunter.hpp"
#include "designations.hpp"
#include "position.hpp"
#include "calendar.hpp"
#include "camera_options.hpp"
#include "claimed_t.hpp"
#include "corpse_harvestable.hpp"
#include "corpse_settler.hpp"
#include "designated_lumberjack.hpp"
#include "explosion_t.hpp"
#include "falling.hpp"
#include "game_stats.hpp"
#include "grazer_ai.hpp"
#include "health.hpp"
#include "initiative.hpp"
#include "machinery/lever.hpp"
#include "lightsource.hpp"
#include "logger.hpp"
#include "natural_attacks_t.hpp"
#include "renderable.hpp"
#include "renderable_composite.hpp"
#include "riding_t.hpp"
#include "sentient_ai.hpp"
#include "settler_ai.hpp"
#include "sleep_clock_t.hpp"
#include "slidemove.hpp"
#include "species.hpp"
#include "stockpile.hpp"
#include "viewshed.hpp"
#include "water_spawner.hpp"
#include "wildlife_group.hpp"
#include "world_position.hpp"
#include "helpers/shift.hpp"
#include "helpers/health_part_t.hpp"
#include "helpers/skill_t.hpp"
#include "item_tags/item_ammo_t.hpp"
#include "item_tags/item_bone_t.hpp"
#include "item_tags/item_chopping_t.hpp"
#include "item_tags/item_digging_t.hpp"
#include "item_tags/item_drink_t.hpp"
#include "item_tags/item_farming.hpp"
#include "item_tags/item_fertilizer_t.hpp"
#include "item_tags/item_food_t.hpp"
#include "item_tags/item_food_prepared_t.hpp"
#include "item_tags/item_hide_t.hpp"
#include "item_tags/item_leather_t.hpp"
#include "item_tags/item_melee_t.hpp"
#include "item_tags/item_ranged_t.hpp"
#include "item_tags/item_seed_t.hpp"
#include "item_tags/item_skull_t.hpp"
#include "item_tags/item_spice_t.hpp"
#include "item_tags/item_topsoil_t.hpp"
#include "items/item.hpp"
#include "items/item_carried.hpp"
#include "items/item_creator.hpp"
#include "items/item_quality.hpp"
#include "items/item_stored.hpp"
#include "items/item_wear.hpp"
#include "mining/designated_miner.hpp"
#include "mining/mining_designations.hpp"
#include "ai_tags/ai_tag_work_deconstruct.hpp"
#include "machinery/sends_signal_t.hpp"
#include "helpers/reaction_task_t.hpp"
#include "flying_t.hpp"
#include "../bengine/ecs.hpp"
#include "machinery/signal_processor_t.hpp"
#include "machinery/float_gauge_t.hpp"
#include "machinery/pressure_plate_t.hpp"
#include "machinery/proximity_sensor_t.hpp"
#include "helpers/standing_orders.hpp"
#include "machinery/oscillator_t.hpp"
#include "machinery/node_graph_position.hpp"
#include "buildings/construct_provides_support.hpp"
#include "hunger_t.hpp"
#include "ai_tags/ai_tag_leisure_eat.hpp"
#include "ai_tags/ai_tag_leisure_drink.hpp"
#include "thirst_t.hpp"

namespace bengine {
	template<class ... Components>
	class ecs_t;

	namespace impl {
		using my_ecs_t = bengine::ecs_t<position_t, designations_t, farming_designations_t, ai_tag_work_farm_plant, ai_tag_work_guarding,
			ai_mode_idle_t, ai_settler_new_arrival_t, ai_tag_leisure_shift_t, ai_tag_my_turn_t, ai_tag_sleep_shift_t, ai_tag_work_architect,
			ai_tag_work_building, ai_tag_work_butcher, ai_tag_work_farm_clear, ai_tag_work_farm_fertilize, ai_tag_work_farm_fixsoil,
			ai_tag_work_farm_water, ai_tag_work_farm_weed, ai_tag_work_harvest, ai_tag_work_hunting, ai_tag_work_lumberjack,
			ai_tag_work_miner, ai_tag_work_order, ai_tag_work_pull_lever, ai_tag_work_shift_t, architecture_designations_t,
			bridge_t, building_t, building_designations_t, construct_container_t, construct_power_t, construct_door_t, construct_provides_sleep_t,
			entry_trigger_t, receives_signal_t, smoke_emitter_t, turret_t, designated_farmer_t, designated_hunter_t,
			calendar_t, camera_options_t, claimed_t, corpse_harvestable, corpse_settler, designated_lumberjack_t, explosion_t,
			falling_t, game_stats_t, grazer_ai, health_t, initiative_t, lever_t, lightsource_t, logger_t, name_t, natural_attacks_t,
			renderable_t, renderable_composite_t, riding_t, sentient_ai, settler_ai_t, sleep_clock_t, slidemove_t, species_t,
			stockpile_t, viewshed_t, water_spawner_t, wildlife_group, world_position_t,
			item_ammo_t, item_bone_t, item_chopping_t, item_digging_t, item_drink_t, item_farming_t, item_fertilizer_t,
			item_food_t, item_hide_t, item_leather_t, item_melee_t, item_ranged_t, item_seed_t, item_skull_t, item_spice_t,
			item_topsoil_t, item_t, item_carried_t, item_creator_t, item_quality_t, item_stored_t, item_wear_t, designated_miner_t,
			mining_designations_t, ai_tag_work_stockpiles_t, item_food_prepared_t, flying_t, ai_tag_work_deconstruct, sends_signal_t, 
			signal_processor_t, float_gauge_t, pressure_plate_t, proximity_sensor_t, oscillator_t, node_graph_position_t, construct_support_t,
			hunger_t, ai_tag_leisure_eat_t, thirst_t, ai_tag_leisure_drink_t
		>;
	}
}

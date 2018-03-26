#include "stdafx.h"
#include "explosive_system.hpp"
#include "../../utils/thread_safe_message_queue.hpp"
#include "../../global_assets/game_pause.hpp"
#include "../../planet/region/region.hpp"
#include "../../raws/plants.hpp"
#include "../../raws/defs/plant_t.hpp"
#include "../../render_engine/chunks/chunks.hpp"
#include "../ai/mining_system.hpp"
#include "../../global_assets/farming_designations.hpp"
#include "../../global_assets/rng.hpp"

using namespace bengine;
using namespace region;
using namespace tile_flags;

namespace systems {
	namespace vegetation {

		struct vegetation_damage_message {
			vegetation_damage_message() {}
			vegetation_damage_message(const int &IDX, const int &dmg) : idx(IDX), damage(dmg) {}
			int idx;
			int damage;
		};

		thread_safe_message_queue<vegetation_damage_message> damage;

		void inflict_damage(const int &idx, const int &dmg) {
			damage.enqueue(vegetation_damage_message{ idx, dmg });
		}

		void run(const double &duration_ms) {
			damage.process_all([](const vegetation_damage_message &msg) {
				damage_vegetation(msg.idx, msg.damage);

				if (veg_hp(msg.idx) < 1 && veg_hp(msg.idx)>0) {
					// We've destroyed the vegetation!
					//std::cout << "Vegetation Destroyed\n";
					set_veg_type(msg.idx, 0);
					calc_render(msg.idx);
					// TODO: emit(map_dirty_message{});
					//chunks::mark_chunk_dirty_by_tileidx(msg.idx);
				}
			});

			if (day_elapsed) {
				region::update_outdoor_calculation();
				for (int z = 0; z<REGION_DEPTH - 1; ++z) {
					for (int y = 0; y<REGION_HEIGHT - 1; ++y) {
						for (int x = 0; x<REGION_WIDTH - 1; ++x) {
							const int idx = mapidx(x, y, z);

							auto farm = farm_designations->farms.find(idx);

							int tick_increase = 1;
							if (farm != farm_designations->farms.end()) {
								if (farm->second.fertilized) tick_increase = 2; // Fertilized plants grow faster
								++farm->second.days_since_watered;
								++farm->second.days_since_weeded;
							}

							if (veg_type(idx) > 0) {
								uint16_t current_tick = veg_ticker(idx) + tick_increase;
								uint8_t current_cycle = veg_lifecycle(idx);
								auto plant = get_plant_def(veg_type(idx));
								if (plant) {
									// Photosynthesizeing plants die in the darkness
									if (plant->requires_light && !region::flag(idx, ABOVE_GROUND)) {
										if (veg_ticker(idx) > 0) {
											current_tick = veg_ticker(idx) - 1;
										}
										else {
											set_veg_type(idx, 0);
											goto escape_loop;
										}
									}
									if (!plant->lifecycle.empty()) {
										//std::cout << "Plant lifecycle ticked\n";
										int return_val = plant->lifecycle[4];
										if (return_val > 3 || return_val < 0) return_val = 0;

										if (current_tick > plant->lifecycle[current_cycle]) {
											++current_cycle;
											current_tick = 0;
											if (current_cycle > 3) current_cycle = return_val;

											if (farm != farm_designations->farms.end() && plant->provides[veg_lifecycle(idx)] != "none") {
												auto[x, y, z] = idxmap(idx);
												farm_designations->harvest.push_back(std::make_pair(false, position_t{ x,y,z }));
											}

											calc_render(idx);
											chunks::mark_chunk_dirty_by_tileidx(idx);
											// TODO: emit(map_dirty_message{});
										}

										// Punish people who don't tend their crops!
										if (farm != farm_designations->farms.end()) {
											if (current_cycle > 0 && farm->second.days_since_watered > 4 && rng.roll_dice(1, 50) <= farm->second.days_since_watered) {
												--current_cycle;
												calc_render(idx);
												chunks::mark_chunk_dirty_by_tileidx(idx);
											}
											if (current_cycle > 0 && farm->second.days_since_weeded > 4 && rng.roll_dice(1, 50) <= farm->second.days_since_weeded) {
												--current_cycle;
												calc_render(idx);
												chunks::mark_chunk_dirty_by_tileidx(idx);
											}
										}

										set_veg_ticker(idx, current_tick);
										set_veg_lifecycle(idx, current_cycle);																			
									}
								}
							}

							escape_loop: {}
						}
					}
				}
			}
		}
	}
}
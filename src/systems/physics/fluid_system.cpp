#include "fluid_system.hpp"
#include "../../messages/tick_message.hpp"
#include "../../main/game_globals.hpp"
#include "../../components/water_spawner.hpp"
#include "../../components/health.hpp"
#include "../../components/game_stats.hpp"
#include "../../messages/inflict_damage_message.hpp"
#include <algorithm>
#include <memory>
#include <thread>

namespace fluids {

int cycle = 0;
std::vector<uint8_t> water_level;
std::unique_ptr<std::thread> fluid_thread;

inline void copy_fluids_to_local() {
    water_level = current_region->water_level;
}

inline void copy_local_to_fluids() {
    current_region->water_level = water_level;
    for (auto i=0; i<REGION_TILES_COUNT; ++i) {
        current_region->calc_render(i);
    }
}

inline void do_falling_water(const int &x, const int &y, const int &z, const int &idx, const int &idx_below, bool &did_something) {
    int idx_bottom = idx_below;
    bool solid = false;
    while (!solid) {
        if (!current_region->solid[idx_bottom - (REGION_HEIGHT*REGION_WIDTH)] && water_level[idx_bottom]<11) {
            idx_bottom -= (REGION_HEIGHT*REGION_WIDTH);
        } else {
            solid = true;
        }
    }
    if (water_level[idx_bottom]<10) {
        --water_level[idx];
        ++water_level[idx_bottom];
        did_something = true;
    }
}

inline void do_water_spread(const int &x, const int &y, const int &z, const int &idx, bool &did_something) {
    std::map<int, int> candidates;
    const int water_here = water_level[idx];

    const std::array<int, 4> directions = { mapidx(x-1, y, z), mapidx(x+1, y, z), mapidx(x, y-1, z), mapidx(x, y+1, z) };

    for (const auto &dir : directions) {
        if (!current_region->solid[dir] && water_level[dir]<10 && water_level[dir] < water_here) candidates.insert({water_level[dir], dir});
    }

    if (!candidates.empty()) {
        for (auto it=candidates.begin(); it!=candidates.end(); ++it) {
            if (water_level[idx] > 0) {
                --water_level[idx];
                ++water_level[it->second];
                did_something = true;
            }
        }
    }
}

inline void do_cell(const int &x, const int &y, const int &z, const int &idx, bool &did_something) {
    std::vector<int> spread_candidates;
    
    const auto idx_below = mapidx(x,y,z-1);
    if (!current_region->solid[idx_below] && current_region->tile_type[idx]!=tile_type::FLOOR && water_level[idx_below] < 11) {
        for (uint8_t i=0; i<current_region->water_level[idx]; ++i) {
            do_falling_water(x, y, z, idx, idx_below, did_something);
        }
    } else if (water_level[idx]>1) {
        for (uint8_t i=0; i<current_region->water_level[idx]; ++i) {
            do_water_spread(x, y, z, idx, did_something);
        }
    }
}

inline void do_layer(const int &z, bool &did_something) {
    for (int y=1; y<REGION_HEIGHT-1; ++y) {
        for (int x=1; x<REGION_WIDTH-1; ++x) {
            const auto idx = mapidx(x,y,z);
            if (water_level[idx]>0) {
                do_cell(x, y, z, idx, did_something);
            }
        }
    }
}

void do_fluids() 
{
    bool did_something = false;

    int start_z, end_z;

    constexpr int ONE_QUARTER = REGION_DEPTH/4;
    constexpr int ONE_HALF = REGION_DEPTH/2;
    constexpr int THREE_QUARTERS = ONE_QUARTER * 3;
    int C = cycle % 4;
    switch (C) {
        case 0 : { start_z = 2; end_z = ONE_QUARTER; } break;
        case 1 : { start_z = ONE_QUARTER; end_z = ONE_HALF; } break;
        case 2 : { start_z = ONE_HALF; end_z = THREE_QUARTERS; } break;
        case 3 : { start_z = THREE_QUARTERS; end_z = REGION_DEPTH-1; } break;
    }

    for (int z=start_z; z<end_z; ++z) {
        do_layer(z, did_something);
    }

    if (did_something) {
        copy_local_to_fluids();
    }

    ++cycle;

    each<water_spawner_t, position_t>([] (entity_t &e, water_spawner_t &w, position_t &pos) {
        const auto idx = mapidx(pos.x, pos.y, pos.z);
        if (w.spawner_type == 1 || w.spawner_type == 2) {
            // TODO: When rainfall is implemented, type 1 only spawns when it rains
            if (current_region->water_level[idx] < 10) current_region->water_level[idx] = 10;
        } else {
            // Type 3 removes water - used to make rivers flow downhill
            if (current_region->water_level[idx] > 0) current_region->water_level[idx] = 0;
        }
    });
}

}

void fluid_system::configure() {
    system_name = "Fluid Dynamics";
    subscribe_mbox<tick_message>();
    subscribe_mbox<slow_tick_message>();
    fluids::water_level.resize(REGION_TILES_COUNT);
}

void fluid_system::update(const double ms) {
    std::queue<slow_tick_message> * sticks = mbox<slow_tick_message>();
	while (!sticks->empty()) {
		sticks->pop();
        for (std::size_t i=0; i<REGION_TILES_COUNT; ++i) {
            if (current_region->water_level[i] == 1 && rng.roll_dice(1,6)==6) current_region->water_level[i] = 0;
        }

        each<position_t>([] (entity_t &e, position_t &pos) {
            if (current_region->water_level[mapidx(pos)] > 7) {
                bool is_drowning = true;

                auto stats = e.component<game_stats_t>();
                if (stats) {
                    if (skill_roll(e.id, stats.get(), rng, "Swimming", DIFFICULTY_AVERAGE) > FAIL) is_drowning = false;
                }

                if (is_drowning) {
                    auto health = e.component<health_t>();
                    if (health) {
                        emit_deferred(inflict_damage_message{e.id, rng.roll_dice(1,4), "Drowning"});
                    }
                }
            }
        });
    }

    std::queue<tick_message> * ticks = mbox<tick_message>();
	while (!ticks->empty()) {
		ticks->pop();
        fluids::copy_fluids_to_local();
        fluids::fluid_thread = std::make_unique<std::thread>(fluids::do_fluids);
	}
}

void fluid_system_end::configure() {}

void fluid_system_end::update(const double duration_ms) {
    if (fluids::fluid_thread) {
        fluids::fluid_thread->join();
        fluids::fluid_thread.reset();
    }
}

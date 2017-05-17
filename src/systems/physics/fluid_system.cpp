#include "fluid_system.hpp"
#include "../../messages/tick_message.hpp"
#include "../../components/water_spawner.hpp"
#include "../../components/health.hpp"
#include "../../components/game_stats.hpp"
#include "../../messages/inflict_damage_message.hpp"
#include "../../messages/map_dirty_message.hpp"
#include "../../main/game_rng.hpp"
#include "../../planet/region/region.hpp"

using namespace region;

std::vector<bool> water_stable(REGION_TILES_COUNT);

namespace fluids {

int cycle = 1;

inline void do_cell(const int &x, const int &y, const int &z, const int &idx, bool &did_something)
{
    water_stable[idx] = true;

    const auto idx_below = mapidx(x,y,z-1);
    // Is there space below? If so, fall
    if (!solid(idx_below) && water_level(idx_below)<10) {
        // Move a water cell down
        add_water(idx_below);
        remove_water(idx);
        calc_render(idx);
        calc_render(idx_below);
        did_something = true;
    } else {
        const uint8_t my_water_level = water_level(idx);
        const int idx_west = mapidx(x-1,y,z);
        const int idx_east = mapidx(x+1,y,z);
        const int idx_north = mapidx(x,y-1,z);
        const int idx_south = mapidx(x,y+1,z);
        if (x>0 && !solid(idx_west) && water_level(idx_west)<my_water_level && water_level(idx_west)<10) {
            add_water(idx_west);
            remove_water(idx);
            calc_render(idx);
            calc_render(idx_west);
            did_something = true;
        } else if (x<REGION_WIDTH-1 && !solid(idx_east) && water_level(idx_east)<my_water_level && water_level(idx_east)<10) {
            add_water(idx_east);
            remove_water(idx);
            calc_render(idx);
            calc_render(idx_east);
            did_something = true;
        } else if (y>0 && !solid(idx_north) && water_level(idx_north)<my_water_level && water_level(idx_north)<10) {
            add_water(idx_north);
            remove_water(idx);
            calc_render(idx);
            calc_render(idx_north);
            did_something = true;
        } else if (y<REGION_HEIGHT-1 && !solid(idx_south) && water_level(idx_south)<my_water_level && water_level(idx_south)<10) {
            add_water(idx_south);
            remove_water(idx);
            calc_render(idx);
            calc_render(idx_south);
            did_something = true;
        }
    }

    // Mark as unstable
    if (did_something) {
        const int idx_west = mapidx(x-1,y,z);
        const int idx_east = mapidx(x+1,y,z);
        const int idx_north = mapidx(x,y-1,z);
        const int idx_south = mapidx(x,y+1,z);
        const int idx_down = mapidx(x,y,z-1);
        const int idx_up = mapidx(x,y,z+1);
        water_stable[idx] = false;
        if (x>0 && water_level(idx_west)>0) water_stable[idx_west] = false;
        if (x<REGION_WIDTH-1 && water_level(idx_east)>0) water_stable[idx_east] = false;
        if (y>0 && water_level(idx_north)>0) water_stable[idx_north] = false;
        if (y<REGION_HEIGHT-1 && water_level(idx_south)>0) water_stable[idx_south] = false;
        if (z>0 && water_level(idx_down)>0) water_stable[idx_down] = false;
        if (z<REGION_DEPTH-1 && water_level(idx_up)>0) water_stable[idx_up] = false;
    }
}

inline void do_layer(const int &z, bool &did_something) {
    for (int y=1; y<REGION_HEIGHT-1; ++y) {
        for (int x=1; x<REGION_WIDTH-1; ++x) {
            const auto idx = mapidx(x,y,z);
            if (water_level(idx)>0 && !water_stable[idx]) {
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
    ++cycle;

    if (did_something) {
        emit(map_dirty_message{});
        emit(map_rerender_message{});
    }

    each<water_spawner_t, position_t>([] (entity_t &e, water_spawner_t &w, position_t &pos) {
        const auto idx = mapidx(pos.x, pos.y, pos.z);
        if (w.spawner_type == 1 || w.spawner_type == 2) {
            // TODO: When rainfall is implemented, type 1 only spawns when it rains
            if (water_level(idx) < 10) set_water_level(idx, 10);
        } else {
            // Type 3 removes water - used to make rivers flow downhill
            if (water_level(idx) > 0) set_water_level(idx, 0);
        }
    });
}

}

void fluid_system::configure() {
    system_name = "Fluid Dynamics";
    subscribe_mbox<tick_message>();
    subscribe_mbox<slow_tick_message>();
    std::fill(water_stable.begin(), water_stable.end(), false);
}

void fluid_system::update(const double ms) {
    std::queue<slow_tick_message> * sticks = mbox<slow_tick_message>();
	while (!sticks->empty()) {
		sticks->pop();
        break;

        for (std::size_t i=0; i<REGION_TILES_COUNT; ++i) {
            if (water_stable[i] && water_level(i) == 1 && rng.roll_dice(1,6)==6) set_water_level(i, 0);
        }

        each<position_t>([] (entity_t &e, position_t &pos) {
            if (water_level(mapidx(pos)) > 7) {
                bool is_drowning = true;

                auto stats = e.component<game_stats_t>();
                if (stats) {
                    if (skill_roll(e.id, *stats, rng, "Swimming", DIFFICULTY_AVERAGE) > FAIL) is_drowning = false;
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
        //fluids::do_fluids();
	}
}

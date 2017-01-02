#include "vegetation_system.hpp"
#include "../messages/vegetation_damage_message.hpp"
#include "../messages/map_dirty_message.hpp"
#include "../messages/tick_message.hpp"
#include "../main/game_globals.hpp"
#include "../raws/plants.hpp"

void vegetation_system::configure() {
    system_name = "Vegetation System";
    subscribe_mbox<vegetation_damage_message>();
    subscribe_mbox<day_elapsed_message>();
}

void vegetation_system::update(const double ms) {
    std::queue<vegetation_damage_message> * vdamage = mbox<vegetation_damage_message>();
    while (!vdamage->empty()) {
        vegetation_damage_message msg = vdamage->front();
        vdamage->pop();

        current_region->tile_hit_points[msg.idx] -= msg.damage;

        if (current_region->tile_hit_points[msg.idx] < 1) {
            // We've destroyed the vegetation!
            //std::cout << "Vegetation Destroyed\n";
            current_region->tile_hit_points[msg.idx] = 0;
            current_region->tile_vegetation_type[msg.idx] = 0;
            current_region->calc_render(msg.idx);
            emit(map_dirty_message{});
        }
    }

    // Grow plants on day ticks
    std::queue<day_elapsed_message> * days = mbox<day_elapsed_message>();
    while (!days->empty()) {
        days->pop();

        for (int z=0; z<REGION_DEPTH-1; ++z) {
            for (int y=0; y<REGION_HEIGHT-1; ++y) {
                for (int x=0; x<REGION_WIDTH-1; ++x) {
                    const int idx = mapidx(x,y,z);
                    if (current_region->tile_vegetation_type[idx] > 0) {
                        uint16_t current_tick = current_region->tile_vegetation_ticker[idx]+1;
                        uint8_t current_cycle = current_region->tile_vegetation_lifecycle[idx];
                        plant_t plant = get_plant_def(current_region->tile_vegetation_type[idx]);
                        const int return_val = plant.lifecycle[4];

                        if (current_tick > plant.lifecycle[current_cycle]) {
                            ++current_cycle;
                            current_tick = 0;
                            if (current_cycle > 3) current_cycle = return_val;
                            current_region->calc_render(idx);
                            emit(map_dirty_message{});
                        }

                        current_region->tile_vegetation_ticker[idx] = current_tick;
                        current_region->tile_vegetation_lifecycle[idx] = current_cycle;
                    }
                }
            }
        }
    }
}

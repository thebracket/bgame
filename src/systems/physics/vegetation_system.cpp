#include "vegetation_system.hpp"
#include "../../messages/vegetation_damage_message.hpp"
#include "../../messages/map_dirty_message.hpp"
#include "../../messages/tick_message.hpp"
#include "../../raws/plants.hpp"
#include "../../planet/region.hpp"

using namespace region;

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

        damage_vegetation(msg.idx, msg.damage);

        if (veg_hp(msg.idx) < 1) {
            // We've destroyed the vegetation!
            //std::cout << "Vegetation Destroyed\n";
            set_veg_type(msg.idx, 0);
            calc_render(msg.idx);
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
                    if (veg_type(idx) > 0) {
                        uint16_t current_tick = veg_ticker(idx)+1;
                        uint8_t current_cycle = veg_lifecycle(idx);
                        plant_t plant = get_plant_def(veg_type(idx));
                        const int return_val = plant.lifecycle[4];

                        if (current_tick > plant.lifecycle[current_cycle]) {
                            ++current_cycle;
                            current_tick = 0;
                            if (current_cycle > 3) current_cycle = return_val;
                            calc_render(idx);
                            emit(map_dirty_message{});
                        }

                        set_veg_ticker(idx, current_tick);
                        set_veg_lifecycle(idx, current_cycle);
                    }
                }
            }
        }
    }
}

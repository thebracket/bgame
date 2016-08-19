#include "vegetation_system.hpp"
#include "../messages/vegetation_damage_message.hpp"
#include "../messages/map_dirty_message.hpp"
#include "../game_globals.hpp"

void vegetation_system::configure() {
    system_name = "Vegetation System";
    subscribe_mbox<vegetation_damage_message>();
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
}

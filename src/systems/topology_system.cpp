#include "topology_system.hpp"
#include "../messages/map_dirty_message.hpp"
#include "../raws/raws.hpp"
#include "../game_globals.hpp"

void topology_system::update(const double duration_ms) {
    std::queue<perform_mining_message> * messages = mbox<perform_mining_message>();
	while (!messages->empty()) {
		perform_mining_message e = messages->front();
		messages->pop();

        switch (e.operation) {
            case 1 : dig(e); break;
            case 2 : channel(e); break;
            case 3 : ramp(e); break;
            case 4 : stairs_up(e); break;
            case 5 : stairs_down(e); break;
            case 6 : stairs_updown(e); break;
            default : throw std::runtime_error("Unknown mining operation: " + std::to_string(e.operation));
        }
        spawn_mining_result(e);
        recalculate(e);
        emit(map_dirty_message{});
    }
}

void topology_system::configure() {
	system_name = "Topology";
    subscribe_mbox<perform_mining_message>();
}

void topology_system::dig(const perform_mining_message &e) {
    current_region->solid[e.target_idx]=false;
    current_region->tile_type[e.target_idx] = tile_type::FLOOR;
    current_region->tile_flags[e.target_idx].set(CAN_STAND_HERE);
}

void topology_system::channel(const perform_mining_message &e) {
    current_region->solid[e.target_idx]=false;
    current_region->tile_type[e.target_idx] = tile_type::OPEN_SPACE;
    
    // Add ramp
    const int below = e.target_idx - (REGION_WIDTH * REGION_HEIGHT);
    current_region->solid[below]=false;
    current_region->tile_type[below] = tile_type::RAMP;
    current_region->tile_flags[below].set(CAN_STAND_HERE);
}

void topology_system::ramp(const perform_mining_message &e) {
    current_region->solid[e.target_idx]=false;
    current_region->tile_type[e.target_idx] = tile_type::RAMP;
    current_region->tile_flags[e.target_idx].set(CAN_STAND_HERE);

    const int above = e.target_idx + (REGION_WIDTH * REGION_HEIGHT);
    current_region->solid[above]=false;
    current_region->tile_type[above] = tile_type::OPEN_SPACE;
    current_region->tile_flags[above].set(CAN_STAND_HERE);
}

void topology_system::stairs_up(const perform_mining_message &e) {
    current_region->solid[e.target_idx]=false;
    current_region->tile_type[e.target_idx] = tile_type::STAIRS_UP;
    current_region->tile_flags[e.target_idx].set(CAN_STAND_HERE);
}

void topology_system::stairs_down(const perform_mining_message &e) {
    current_region->solid[e.target_idx]=false;
    current_region->tile_type[e.target_idx] = tile_type::STAIRS_DOWN;
    current_region->tile_flags[e.target_idx].set(CAN_STAND_HERE);
}

void topology_system::stairs_updown(const perform_mining_message &e) {
    current_region->solid[e.target_idx]=false;
    current_region->tile_type[e.target_idx] = tile_type::STAIRS_UPDOWN;
    current_region->tile_flags[e.target_idx].set(CAN_STAND_HERE);
}

void topology_system::recalculate(const perform_mining_message &e) {
    for (int Z=-2; Z<3; ++Z) {
        for (int Y=-2; Y<3; ++Y) {
            for (int X=-2; X<3; ++X) {
                current_region->tile_calculate(e.x + X, e.y + Y, e.z + Z);
                current_region->tile_calculate(e.x + X, e.y + Y, e.z + Z);
            }
        }
    }
}

void topology_system::spawn_mining_result_impl(const perform_mining_message &e, std::string tag) {
    if (tag == "") return;

    auto finder = item_defs.find(tag);
    if (finder != item_defs.end()) {
        //std::cout << "Topology system - producing a [" << tag << "]";
        spawn_item_on_ground(e.x, e.y, e.z, tag);
    } else {
        std::cout << "Topology system - don't know how to spawn a [" << tag << "]\n";
    }
}

void topology_system::spawn_mining_result(const perform_mining_message &e) {
    const std::string mining_result = material_defs[current_region->tile_material[e.target_idx]].mines_to_tag;
    const std::string mining_result2 = material_defs[current_region->tile_material[e.target_idx]].mines_to_tag_second;

    //std::cout << "Topology - mines to [" << mining_result << "], [" << mining_result2 << "]";

    spawn_mining_result_impl(e, mining_result);
    spawn_mining_result_impl(e, mining_result2);
}
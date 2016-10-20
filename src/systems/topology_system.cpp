#include "topology_system.hpp"
#include "../messages/map_dirty_message.hpp"
#include "../messages/recalculate_mining_message.hpp"
#include "../messages/renderables_changed_message.hpp"
#include "../raws/raws.hpp"
#include "../main/game_globals.hpp"

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
        emit(recalculate_mining_message{});
    }

    std::queue<perform_construction_message> * cmessages = mbox<perform_construction_message>();
    while (!cmessages->empty()) {
        perform_construction_message e = cmessages->front();
        cmessages->pop();

        build_construction(e);
    }
}

void topology_system::configure() {
	system_name = "Topology";
    subscribe_mbox<perform_mining_message>();
    subscribe_mbox<perform_construction_message>();
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
    if (current_region->solid[below]) {
        current_region->solid[below]=false;
        current_region->tile_type[below] = tile_type::RAMP;
        current_region->tile_flags[below].set(CAN_STAND_HERE);
    }
}

void topology_system::ramp(const perform_mining_message &e) {
    current_region->solid[e.target_idx]=false;
    current_region->tile_type[e.target_idx] = tile_type::RAMP;
    current_region->tile_flags[e.target_idx].set(CAN_STAND_HERE);

    const int above = e.target_idx + (REGION_WIDTH * REGION_HEIGHT);
    if (current_region->solid[above]) {
        current_region->solid[above]=false;
        current_region->tile_type[above] = tile_type::OPEN_SPACE;
        current_region->tile_flags[above].set(CAN_STAND_HERE);
    }
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
                current_region->revealed[mapidx(e.x + X, e.y + Y, e.z + Z)] = true;
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
        std::size_t material = current_region->tile_material[e.target_idx];
        if (tag == "ore") {
            for (const std::string &metal : material_defs[material].ore_materials) {
                auto metal_finder = material_defs_idx.find(metal);
                if (metal_finder != material_defs_idx.end()) {
                    spawn_item_on_ground(e.x, e.y, e.z, tag, metal_finder->second);
                } else {
                    std::cout << "WARNING: Tried to spawn " << metal << ", but it has no material definition\n";
                }
            }
        } else {
            spawn_item_on_ground(e.x, e.y, e.z, tag, material);
        }
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

void topology_system::build_construction(const perform_construction_message &e) {
    std::cout << "Received a build construction message\n";
    std::cout << "Entity ID: " << e.entity_id << "\n";
    std::cout << "Tag: " << e.tag << "\n";
    std::cout << "Material: " << e.material << "\n";

    auto construction_pos = entity(e.entity_id)->component<position_t>();
    const int index = mapidx(construction_pos->x, construction_pos->y, construction_pos->z);
    auto finder = building_defs.find(e.tag);
    for (const building_provides_t &provides : finder->second.provides) {
        
        if (provides.provides == provides_wall) {
            current_region->tile_type[index] = tile_type::WALL;
            // Relocate anything stuck in the new wall
            each<position_t>([index] (entity_t &E, position_t &P) {
                if (mapidx(P.x, P.y, P.z) == index) {
                    // Something needs moving!
                    if (!current_region->solid[index+1]) {
                        std::tie(P.x, P.y, P.z) = idxmap(index+1);
                    } else if (!current_region->solid[index-1]) {
                        std::tie(P.x, P.y, P.z) = idxmap(index-1);
                    } else if (!current_region->solid[index+REGION_WIDTH]) {
                        std::tie(P.x, P.y, P.z) = idxmap(index+REGION_WIDTH);
                    } else if (!current_region->solid[index-REGION_WIDTH]) {
                        std::tie(P.x, P.y, P.z) = idxmap(index-REGION_WIDTH);
                    }
                }
            });
        } else if (provides.provides == provides_floor) {
            current_region->tile_type[index] = tile_type::FLOOR;
        } else if (provides.provides == provides_stairs_up) {
            current_region->tile_type[index] = tile_type::STAIRS_UP;
        } else if (provides.provides == provides_stairs_down) {
            current_region->tile_type[index] = tile_type::STAIRS_DOWN;
        } else if (provides.provides == provides_stairs_updown) {
            current_region->tile_type[index] = tile_type::STAIRS_UPDOWN;
        } else if (provides.provides == provides_ramp) {
            current_region->tile_type[index] = tile_type::RAMP;
        }
    }
    current_region->tile_material[index] = e.material;

    current_region->tile_calculate(construction_pos->x, construction_pos->y, construction_pos->z);
    for (int Z=-2; Z<3; ++Z) {
        for (int Y=-2; Y<3; ++Y) {
            for (int X=-2; X<3; ++X) {
                current_region->tile_calculate(construction_pos->x + X, construction_pos->y + Y, construction_pos->z + Z);
            }
        }
    }

    delete_entity(e.entity_id);
    std::cout << "Deleted entity\n";
    emit(renderables_changed_message{});
}

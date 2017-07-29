#include "topology_system.hpp"
#include "../../messages/map_dirty_message.hpp"
#include "../../messages/renderables_changed_message.hpp"
#include "../../raws/raws.hpp"
#include "../../raws/items.hpp"
#include "../../raws/defs/item_def_t.hpp"
#include "../../raws/materials.hpp"
#include "../../raws/buildings.hpp"
#include "../../raws/defs/building_def_t.hpp"
#include "../../components/entry_trigger.hpp"
#include "../../components/receives_signal.hpp"
#include "../../planet/region/region.hpp"
#include "../../raws/defs/material_def_t.hpp"

using namespace region;

void topology_system::update(const double duration_ms) {
}

void topology_system::configure() {
	system_name = "Topology";

    subscribe<perform_mining_message>([this] (perform_mining_message &e) {
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
        emit(map_changed_message{});
    });

    subscribe<perform_construction_message>([this] (perform_construction_message &e) {
        build_construction(e);
    });
}

void topology_system::dig(const perform_mining_message &e) {
    make_floor(e.target_idx);
    int x,y,z;
    std::tie(x,y,z) = idxmap(e.target_idx);
    emit_deferred(tile_removed_message{x,y,z});
}

void topology_system::channel(const perform_mining_message &e) {
    make_open_space(e.target_idx);
    int x,y,z;
    std::tie(x,y,z) = idxmap(e.target_idx);
    emit_deferred(tile_removed_message{x,y,z});
    
    // Add ramp
    const int below = e.target_idx - (REGION_WIDTH * REGION_HEIGHT);
    if (solid(below)) {
        make_ramp(below);
    }
}

void topology_system::ramp(const perform_mining_message &e) {
    make_ramp(e.target_idx);

    const int above = e.target_idx + (REGION_WIDTH * REGION_HEIGHT);
    if (solid(above)) {
        make_open_space(above);
        set_flag(above, CAN_STAND_HERE);
    }
}

void topology_system::stairs_up(const perform_mining_message &e) {
    make_stairs_up(e.target_idx);
}

void topology_system::stairs_down(const perform_mining_message &e) {
    make_stairs_down(e.target_idx);
}

void topology_system::stairs_updown(const perform_mining_message &e) {
    make_stairs_updown(e.target_idx);
}

void topology_system::recalculate(const perform_mining_message &e) {
    for (int Z=-2; Z<3; ++Z) {
        for (int Y=-2; Y<3; ++Y) {
            for (int X=-2; X<3; ++X) {
                if (e.x + X > 0 && e.x + X < REGION_WIDTH && e.y+Y > 0 && e.y + Y < REGION_HEIGHT && e.z +Z > 0 && e.z+Z<REGION_DEPTH) {
                    reveal(mapidx(e.x + X, e.y + Y, e.z + Z));
                    tile_calculate(e.x + X, e.y + Y, e.z + Z);
                }
            }
        }
    }
}

void topology_system::spawn_mining_result_impl(const perform_mining_message &e, std::string tag) {
    if (tag == "") return;

    int X,Y,Z;
    std::tie(X,Y,Z) = idxmap(e.target_idx);

    auto finder = get_item_def(tag);
    if (finder != nullptr) {
        //std::cout << "Topology system - producing a [" << tag << "]";
        auto mat = material(e.target_idx);
        if (tag == "ore") {
            for (const std::string &metal : get_material(mat)->ore_materials) {
                const auto metal_finder = get_material_by_tag(metal);
                spawn_item_on_ground(X, Y, Z, tag, metal_finder);
            }
        } else {
            spawn_item_on_ground(X, Y, Z, tag, mat);
        }
    } else {
        std::cout << "Topology system - don't know how to spawn a [" << tag << "]\n";
    }
}

void topology_system::spawn_mining_result(const perform_mining_message &e) {
    const std::string mining_result = get_material(material(e.target_idx))->mines_to_tag;
    const std::string mining_result2 = get_material(material(e.target_idx))->mines_to_tag_second;

    //std::cout << "Topology - mines to [" << mining_result << "], [" << mining_result2 << "]";

    spawn_mining_result_impl(e, mining_result);
    spawn_mining_result_impl(e, mining_result2);
}

void topology_system::build_construction(const perform_construction_message &e) {
    std::cout << "Received a build construction message\n";
    std::cout << "Entity ID: " << e.entity_id << "\n";
    std::cout << "Tag: " << e.tag << "\n";
    std::cout << "Material: " << e.material << "\n";

    bool entity_should_be_deleted = true;
    auto construction_pos = entity(e.entity_id)->component<position_t>();
    const int index = mapidx(construction_pos->x, construction_pos->y, construction_pos->z);
    auto finder = get_building_def(e.tag);
    for (const building_provides_t &provides : finder->provides) {
        
        if (provides.provides == provides_wall) {
            set_tile_type(index, tile_type::WALL);
            // Relocate anything stuck in the new wall
            each<position_t>([index] (entity_t &E, position_t &P) {
                if (mapidx(P.x, P.y, P.z) == index) {
                    // Something needs moving!
                    if (!solid(index+1)) {
                        std::tie(P.x, P.y, P.z) = idxmap(index+1);
                    } else if (!solid(index-1)) {
                        std::tie(P.x, P.y, P.z) = idxmap(index-1);
                    } else if (!solid(index+REGION_WIDTH)) {
                        std::tie(P.x, P.y, P.z) = idxmap(index+REGION_WIDTH);
                    } else if (!solid(index-REGION_WIDTH)) {
                        std::tie(P.x, P.y, P.z) = idxmap(index-REGION_WIDTH);
                    }
                }
            });
        } else if (provides.provides == provides_floor) {
            set_tile_type(index, tile_type::FLOOR);
        } else if (provides.provides == provides_stairs_up) {
            set_tile_type(index, tile_type::STAIRS_UP);
        } else if (provides.provides == provides_stairs_down) {
            set_tile_type(index, tile_type::STAIRS_DOWN);
        } else if (provides.provides == provides_stairs_updown) {
            set_tile_type(index, tile_type::STAIRS_UPDOWN);
        } else if (provides.provides == provides_ramp) {
            set_tile_type(index, tile_type::RAMP);
        } else if (provides.provides == provides_cage_trap) {
            // Create a new entity for the trap
            // Add an entry_trigger and a position to it
            int x,y,z;
            std::tie(x,y,z) = idxmap(index);
            create_entity()->assign(position_t{x, y, z})->assign(entry_trigger_t{trigger_cage});
            emit_deferred(triggers_changes_message{});
            entity_should_be_deleted = false;
        } else if (provides.provides == provides_stonefall_trap) {
            // Create a new entity for the trap
            // Add an entry_trigger and a position to it
            int x,y,z;
            std::tie(x,y,z) = idxmap(index);
            create_entity()->assign(position_t{x, y, z})->assign(entry_trigger_t{trigger_stonefall});
            emit_deferred(triggers_changes_message{});
            entity_should_be_deleted = false;
        } else if (provides.provides == provides_blades_trap) {
            // Create a new entity for the trap
            // Add an entry_trigger and a position to it
            int x,y,z;
            std::tie(x,y,z) = idxmap(index);
            create_entity()->assign(position_t{x, y, z})->assign(entry_trigger_t{trigger_blade});
            emit_deferred(triggers_changes_message{});
            entity_should_be_deleted = false;
        } else if (provides.provides == provides_spikes) {
            // Create a new entity for the trap
            // Add an entry_trigger and a position to it
            int x,y,z;
            std::tie(x,y,z) = idxmap(index);
            create_entity()->assign(position_t{x, y, z})->assign(receives_signal_t{});
            entity_should_be_deleted = false;
        }
    }
    set_tile_material(index, e.material);

    tile_calculate(construction_pos->x, construction_pos->y, construction_pos->z);
    for (int Z=-2; Z<3; ++Z) {
        for (int Y=-2; Y<3; ++Y) {
            for (int X=-2; X<3; ++X) {
                tile_calculate(construction_pos->x + X, construction_pos->y + Y, construction_pos->z + Z);
            }
        }
    }

    if (entity_should_be_deleted) {
        delete_entity(e.entity_id);
        std::cout << "Deleted entity\n";
    }
    emit(renderables_changed_message{});
    emit(map_changed_message{});
}
